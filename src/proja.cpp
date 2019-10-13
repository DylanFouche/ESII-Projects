
#include "proja.h"

using namespace std;

void cleanupGPIO(int dum) {
	pinMode(RESET_BTN, INPUT);
    pinMode(ALARM_DISMISS_BTN, INPUT);
    pinMode(INTERVAL_ADJUST_BTN, INPUT);
    pinMode(START_STOP_BTN, INPUT);
    pinMode(ALARM_LED, INPUT);
	exit(1);
}

/*
 * main function
 */
int main()
{
    signal(SIGINT, cleanupGPIO);

    setup_gpio();
    setup_dac();
    //TODO:
    //setup adc
    //setup rtc
    //setup blynk

	pthread_attr_t tattr;
	pthread_t thread_id;
	int newprio = 99;
	sched_param param;

	pthread_attr_init (&tattr);
	pthread_attr_getschedparam (&tattr, &param); /* safe to get existing scheduling param */
	param.sched_priority = newprio; /* set the priority; others are unchanged */
	pthread_attr_setschedparam (&tattr, &param); /* setting the new scheduling param */
	pthread_create(&thread_id, &tattr, adc_read_thread, (void *)1); /* with new priority specified */
	
    while(true){
        //TODO:
        //read in from adc
        //do some computation
        float v_out = (light / (float)1023) * temp;

        if (v_out < MIN_THRESH || v_out > MAX_THRESH)
            activate_alarm();
        //write out to dac

        //Write to console
        printf("%f C | %d | %f V\n", temp, light, v_out);
        //publish data to blynk

        delay(1000);
    }

    //Join and exit the reading thread
	pthread_join(thread_id, NULL);
    pthread_exit(NULL);

    return 0;
}

/*
 * Utility functions
 */
void write_to_dac(char Vout)
{
    char reg[2];
    reg[0] = 0b01010000 | (Vout>>4);
    reg[1] = Vout<<4;
    wiringPiSPIDataRW(DAC_SPI_CHAN, (unsigned char*)reg, 2);
}
int read_adc_channel(int channel)
{
    char reg[3];
    reg[0] = 1;
    reg[1] = (8 + channel) << 4;
    reg[2] = 0;
    wiringPiSPIDataRW(ADC_SPI_CHAN, (unsigned char*)reg, 3);
    return ((reg[1]&3) << 8) + reg[2];
}
float get_degrees_celsius(int data)
{
    float v_out = (data * 330) / (float)1023;
    return v_out - 50;
}
void *adc_read_thread(void *threadargs)
{
    while (!stopped)
	{
		temp = get_degrees_celsius(read_adc_channel(TEMP_CHAN));
		light = read_adc_channel(LIGHT_CHAN);
	}

    pthread_exit(NULL);
}
void activate_alarm(void)
{
    long alarm = millis();
    if (alarm - last_alarm > ALARM_INTERVAL || last_alarm == 0){
        digitalWrite(ALARM_LED,1);
        last_alarm = alarm;
    }
}
void deactivate_alarm(void)
{
    digitalWrite(ALARM_LED,0);
}
/*
 * Interrupt handlers
 */
void reset_isr(void){
    long interrupt_time = millis();
    if(interrupt_time - last_interrupt_a > DEBOUNCE_TIME){
        cout << "Reset button pressed" << endl;
    }
    last_interrupt_a = interrupt_time;
}
void alarm_dismiss_isr(void){
    long interrupt_time = millis();
    if(interrupt_time - last_interrupt_b > DEBOUNCE_TIME){
        cout << "Alarm dismiss button pressed" << endl;
        deactivate_alarm();
    }
    last_interrupt_b = interrupt_time;
}
void interval_adjust_isr(void){
    long interrupt_time = millis();
    if(interrupt_time - last_interrupt_c > DEBOUNCE_TIME){
        cout << "Interval adjust button pressed" << endl;
    }
    last_interrupt_c = interrupt_time;
}
void start_stop_isr(void){
    long interrupt_time = millis();
    if(interrupt_time - last_interrupt_d > DEBOUNCE_TIME){
        cout << "Start/stop button pressed" << endl;
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

void setup_dac(void)
{
    //set up the SPI interface
    wiringPiSPISetup(DAC_SPI_CHAN, SPI_SPEED);
    wiringPiSPISetup(ADC_SPI_CHAN, SPI_SPEED);
}
