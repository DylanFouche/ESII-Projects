#ifndef _PROJ_A_BLYNK_H
#define _PROJ_A_BLYNK_H

void setup_blynk(void);
void write_to_blynk(float humid, float temp, int light, bool alarm_on);
void update_blynk_time(int HH, int MM, int SS);

#endif