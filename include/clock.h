#ifndef _PROJ_A_CLOCK_H
#define _PROJ_A_CLOCK_H

void setup_RTC(void);
void start_sys_timer(void);
void get_current_time(int & HH, int & MM, int & SS);
void get_sys_time(int & hh, int & mm, int & ss);
int hexCompensation(int units);
int decCompensation(int units);

#endif