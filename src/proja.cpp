
#include "proja.h"

using namespace std;

/*
 * main function
 */
int main()
{
    setup_gpio();
    setup_dac();
    //TODO:
    //setup adc
    //setup rtc
    //setup blynk
    while(true){
      //TODO:
      //read in from adc
      //do some computation
      //write out to dac
      //publish data to blynk
    }
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
void activate_alarm(void)
{
  digitalWrite(ALARM_LED,1);
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
