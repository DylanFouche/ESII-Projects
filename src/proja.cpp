#include "proja.h"

using namespace std;

void cleanupGPIO(int dum) {
    reading = false;
    stopped = true;

	pinMode(RESET_BTN, INPUT);
    pinMode(ALARM_DISMISS_BTN, INPUT);
    pinMode(INTERVAL_ADJUST_BTN, INPUT);
    pinMode(START_STOP_BTN, INPUT);
    pinMode(ALARM_LED, INPUT);
	exit(0);
}

/*
 * main function
 */
int main()
{
    signal(SIGINT, cleanupGPIO); //Exit on Control-C

    //set up WiringPi
    setup_gpio();
    setup_dac_adc();

    //set up Blynk
    setup_blynk();

    //set up mosquitto
    mqtt = new mosquito_wrapper(MQTT_ID, MQTT_ADDRESS, MQTT_PORT);

    //set up pthread for adc
    pthread_attr_t tattr;
    pthread_t thread_id;
    int newprio = 99;
    sched_param param;
    pthread_attr_init (&tattr);
    pthread_attr_getschedparam (&tattr, &param); /* safe to get existing scheduling param */
    param.sched_priority = newprio; /* set the priority; others are unchanged */
    pthread_attr_setschedparam (&tattr, &param); /* setting the new scheduling param */
    pthread_create(&thread_id, &tattr, adc_read_thread, (void *)1); /* with new priority specified */

    start_sys_timer(); //start sys timer on RTC

    printf("| RTC Time | Sys Timer | Humidity | Temperature | Light | DAC_Vout | Alarm |\n");

    while(true){
        //compute Vout
        float v_out = (light / (float)1023) * humid;

        //check if we need to activate alarm
        if (v_out < MIN_THRESH || v_out > MAX_THRESH)
            activate_alarm();

        //write out to dac
        int level = (v_out / 3.3) * 512;
	write_to_dac(level);

        //Get current times
        get_current_time(HH, MM, SS);
        get_sys_time(hh, mm, ss);
        update_blynk_time(HH, MM, SS);

        //Write to console
        printf("| %d:%d:%d | %d:%d:%d | ", HH, MM, SS, hh, mm, ss);
        printf("%.2f V | %d C | %d | %.2f V | ", humid, temp, light, v_out);
        printf("%c |\n", (alarm_on) ? '*' : ' ');

        //publish data to blynk
	write_to_blynk(humid, temp, light, alarm_on);

	//publish data to mqtt
	mqtt->write_to_mqtt(humid, temp, light, alarm_on);
	mqtt->loop();

        //wait specified time
        delay(delay_times[delay_i]);
    }

    //Join and exit the reading thread
    pthread_join(thread_id, NULL);
    pthread_exit(NULL);

    return 0;
}

/*
 * Utility functions
 */

// Write data to DAC
void write_to_dac(int level)
{
    level = level & 0b1111111111;
    char reg[2];
    reg[0] = 0b01010000 | (level>>4);
    reg[1] = level<<2;
    wiringPiSPIDataRW(DAC_SPI_CHAN, (unsigned char*)reg, 2);
}

// Read data from channel of ADC
int read_adc_channel(int channel)
{
    char reg[3];
    reg[0] = 1;
    reg[1] = (8 + channel) << 4;
    reg[2] = 0;
    wiringPiSPIDataRW(ADC_SPI_CHAN, (unsigned char*)reg, 3);
    return ((reg[1]&3) << 8) + reg[2];
}

// Convert data to voltage
float get_volts(int data)
{
    return (data * 3.3) / (float)1023;
}
float get_degrees_celsius(int data)
{
    return (data * 330) / (float)1023;
}
void *adc_read_thread(void *threadargs)
{
    while (!stopped)
	{
        while (!reading) continue;

		temp = get_degrees_celsius(read_adc_channel(TEMP_CHAN));
		light = read_adc_channel(LIGHT_CHAN);
		humid = get_volts(read_adc_channel(HUMID_CHAN));
	}

    pthread_exit(NULL);
}
void activate_alarm(void)
{
    long alarm = millis();
    if (alarm - last_alarm > ALARM_INTERVAL || last_alarm == 0){
        digitalWrite(ALARM_LED,1);
        alarm_on = true;
        last_alarm = alarm;
    }
}
void deactivate_alarm(void)
{
    digitalWrite(ALARM_LED,0);
    alarm_on = false;
}

/*
 * Interrupt handlers
 */
void reset_isr(void){
    long interrupt_time = millis();
    if(interrupt_time - last_interrupt_a > DEBOUNCE_TIME){
        last_alarm = 0;
        system("clear");
        printf("RTC Time | Sys Timer | Humidity | Temperature | Light | DAC_Vout | Alarm\n");
        start_sys_timer();
    }
    last_interrupt_a = interrupt_time;
}
void alarm_dismiss_isr(void){
    long interrupt_time = millis();
    if(interrupt_time - last_interrupt_b > DEBOUNCE_TIME){
        deactivate_alarm();
    }
    last_interrupt_b = interrupt_time;
}
void interval_adjust_isr(void){
    long interrupt_time = millis();
    if(interrupt_time - last_interrupt_c > DEBOUNCE_TIME){
        if (delay_i == 2)
            delay_i = 0;
        else
            delay_i++;
    }
    last_interrupt_c = interrupt_time;
}
void start_stop_isr(void){
    long interrupt_time = millis();
    if(interrupt_time - last_interrupt_d > DEBOUNCE_TIME){
        reading = !reading;
    }
    last_interrupt_d = interrupt_time;
}
/*
 * Setup functions
 */
void setup_gpio(void)
{
    //set up wiring Pi
    wiringPiSetup();
    setup_RTC();

    //set up the buttons
    pinMode(RESET_BTN, INPUT);
    pinMode(ALARM_DISMISS_BTN, INPUT);
    pinMode(INTERVAL_ADJUST_BTN, INPUT);
    pinMode(START_STOP_BTN, INPUT);
    pullUpDnControl(RESET_BTN, PUD_UP);
    pullUpDnControl(ALARM_DISMISS_BTN, PUD_UP);
    pullUpDnControl(INTERVAL_ADJUST_BTN, PUD_UP);
    pullUpDnControl(START_STOP_BTN, PUD_UP);
    //register interrupt handlers for buttons
    wiringPiISR(RESET_BTN, INT_EDGE_RISING, &reset_isr);
    wiringPiISR(ALARM_DISMISS_BTN, INT_EDGE_RISING, &alarm_dismiss_isr);
    wiringPiISR(INTERVAL_ADJUST_BTN, INT_EDGE_RISING, &interval_adjust_isr);
    wiringPiISR(START_STOP_BTN, INT_EDGE_RISING, &start_stop_isr);
    //set up led
    pinMode(ALARM_LED, OUTPUT);
}

void setup_dac_adc(void)
{
    //set up the SPI interface
    wiringPiSPISetup(DAC_SPI_CHAN, SPI_SPEED);
    wiringPiSPISetup(ADC_SPI_CHAN, SPI_SPEED);
}
