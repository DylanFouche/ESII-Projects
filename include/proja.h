
#ifndef _PROJ_A_H
#define _PROJ_A_H

//Includes
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <softPwm.h>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <pthread.h>
#include <iostream>
#include <pthread.h>
#include <signal.h> // For cleaning up GPIO

//setting up BLYNK
#define BLYNK_PRINT stdout

#include <BlynkApiWiringPi.h>
#include <BlynkSocket.h>
#include <BlynkOptionsParser.h>

static BlynkTransportSocket _blynkTransport;
BlynkSocket Blynk(_blynkTransport);
#include <BlynkWidgets.h>
//BlynkTimer timer;


//Define buttons and leds
#define ALARM_LED 1
#define RESET_BTN 2
#define ALARM_DISMISS_BTN 3
#define INTERVAL_ADJUST_BTN 0
#define START_STOP_BTN 7

//SPI Settings
#define SPI_SPEED 204800
#define DAC_SPI_CHAN 0
#define ADC_SPI_CHAN 1
#define TEMP_CHAN 0
#define LIGHT_CHAN 1
#define DAC_CHAN 3

//Debounce time
#define DEBOUNCE_TIME 200

//Alarm thresholds
#define MIN_THRESH 0.65
#define MAX_THRESH 2.65

//Function definitions
void setup_gpio(void);
void setup_dac(void);
int read_adc_channel(int channel);
float get_degrees_celsius(int data);
void *adc_read_thread(void *threadargs);
void activate_alarm(void);
void deactivate_alarm(void);
void write_to_dac(char Vout);
int main(void);

//global variables for debouncing
long last_interrupt_a = 0;
long last_interrupt_b = 0;
long last_interrupt_c = 0;
long last_interrupt_d = 0;

//ADC readings
float humid;
float temp;
int light;
float dac_out;

//Flag used by ADC reading thread
bool stopped = false;

#endif
