
#ifndef _PROJ_A_H
#define _PROJ_A_H

//Includes
#include <wiringPi.h>
#include <wiringPiSPI.h>
// #include <wiringPiI2C.h>
#include <softPwm.h>
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <pthread.h>
#include <iostream>
#include <pthread.h>
#include <signal.h> // For cleaning up GPIO
#include <time.h>

//setting up BLYNK
#define BLYNK_PRINT stdout

#include <BlynkApiWiringPi.h>
#include <BlynkSocket.h>
#include <BlynkOptionsParser.h>

static BlynkTransportSocket _blynkTransport;
BlynkSocket Blynk(_blynkTransport);
#include <BlynkWidgets.h>

char AUTH_TOKEN[] = "pZn612Bmfqu9_3344QW0NE6sAWEkkj94";

//define BLYNK virtual pins
#define SYSTEM_TIME V1
#define HUMIDITY V2
#define TEMP V3
#define LIGHT V4
#define ALARM V5

//Define buttons and leds
#define ALARM_LED 1
#define RESET_BTN 2
#define ALARM_DISMISS_BTN 3
#define INTERVAL_ADJUST_BTN 0
#define START_STOP_BTN 7

//Debounce time
#define DEBOUNCE_TIME 200

//SPI Settings
#define SPI_SPEED 204800
#define DAC_SPI_CHAN 0
#define ADC_SPI_CHAN 1
#define TEMP_CHAN 0
#define LIGHT_CHAN 1
#define HUMID_CHAN 2
#define DAC_CHAN 3

//Alarm variables
#define ALARM_INTERVAL 3000
#define MIN_THRESH 0.65
#define MAX_THRESH 2.65

//Function definitions
void setup_gpio(void);
void cleanupGPIO(int dum);
void setup_dac_adc(void);
int read_adc_channel(int channel);
float get_degrees_celsius(int data);
void *adc_read_thread(void *threadargs);
void activate_alarm(void);
void deactivate_alarm(void);
void write_to_dac(char Vout);
void write_to_blynk(void);
void get_current_time(void);
int main(void);
// int hexCompensation(int units);

//global variables for debouncing
long last_interrupt_a = 0;
long last_interrupt_b = 0;
long last_interrupt_c = 0;
long last_interrupt_d = 0;

long last_alarm = 0;

//Readings
float humid;
float temp;
int light;
float dac_out;
bool alarm_on = false;
char system_time[8];

//Flag used by ADC reading thread
bool stopped = false;
bool reading = true;

//RTC
int HH, MM, SS; //System time
int hh, mm, ss; //Start time

// // define constants
// const char RTCAddr = 0x6f;
// const char SEC = 0x00; // see register table in datasheet
// const char MIN = 0x01;
// const char HOUR = 0x02;

#endif
