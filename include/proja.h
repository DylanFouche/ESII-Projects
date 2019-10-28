
#ifndef _PROJ_A_H
#define _PROJ_A_H

//Includes
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <softPwm.h>
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <pthread.h>
#include <iostream>
#include <signal.h> // For cleaning up GPIO

#include "clock.h"
#include "blynk.h"
#include "mosquito.h"

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
#define ALARM_INTERVAL 180000
float MIN_THRESH = 0.65;
float MAX_THRESH = 2.65;

//MQTT variables
#define MQTT_ID "ESII_PROJB"
#define MQTT_ADDRESS "test.mosquitto.org"
#define MQTT_PORT 1883
mosquito_wrapper* mqtt;

//Function definitions
void setup_gpio(void);
void cleanupGPIO(int dum);
void setup_dac_adc(void);
int read_adc_channel(int channel);
float get_degrees_celsius(int data);
void *adc_read_thread(void *threadargs);
void activate_alarm(void);
void deactivate_alarm(void);
void write_to_dac(int level);
void write_to_blynk(void);
void update_blynk_time(void);
int main(void);

// Delay between readings displayed
int delay_i = 0, delay_times[3] = {1000, 2000, 5000};

//global variables for debouncing
long last_interrupt_a = 0;
long last_interrupt_b = 0;
long last_interrupt_c = 0;
long last_interrupt_d = 0;

long last_alarm = 0;

//Readings
float humid;
int temp;
int light;
float dac_out;
bool alarm_on = false;

//Flag used by ADC reading thread
bool stopped = false;
bool reading = true;

//RTC
int HH, MM, SS; //System time
int hh, mm, ss; //RTC time

#endif
