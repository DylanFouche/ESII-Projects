#ifndef CLOCK_H
#define CLOCK_H

#include <time.h>
#include <wiringPiI2C.h>

void start_sys_timer(void);
void get_current_time(int & HH, int & MM, int && SS);
int hexCompensation(int units);
int decCompensation(int units);

// define constants
const char RTCAddr = 0x6f;
const char SEC = 0x00; // see register table in datasheet
const char MIN = 0x01;
const char HOUR = 0x02;

#endif