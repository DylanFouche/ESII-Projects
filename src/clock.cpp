#include "clock.h"

void start_sys_timer(void)
{
    int hours = 0;
    hours = decCompensation(hours);
    wiringPiI2CWriteReg8(RTC, HOUR, hours);

    int mins = decCompensation(0);
    wiringPiI2CWriteReg8(RTC, MIN, mins);

    int secs = decCompensation(0);
    wiringPiI2CWriteReg8(RTC, SEC, 0b10000000+secs);
}

void get_current_time(int & HH, int & MM, int && SS)
{
    time_t rawtime;
    struct tm * timeinfo;
    time (&rawtime);
    timeinfo = localtime(&rawtime);
    HH = timeinfo ->tm_hour;
    MM = timeinfo ->tm_min;
    SS = timeinfo ->tm_sec;
}

int hexCompensation(int units){
	/*Convert HEX or BCD value to DEC where 0x45 == 0d45 
	  This was created as the lighXXX functions which determine what GPIO pin to set HIGH/LOW
	  perform operations which work in base10 and not base16 (incorrect logic) 
	*/
	int unitsU = units%0x10;

	if (units >= 0x50){
		units = 50 + unitsU;
	}
	else if (units >= 0x40){
		units = 40 + unitsU;
	}
	else if (units >= 0x30){
		units = 30 + unitsU;
	}
	else if (units >= 0x20){
		units = 20 + unitsU;
	}
	else if (units >= 0x10){
		units = 10 + unitsU;
	}
	return units;
}

int decCompensation(int units){
	int unitsU = units%10;

	if (units >= 50){
		units = 0x50 + unitsU;
	}
	else if (units >= 40){
		units = 0x40 + unitsU;
	}
	else if (units >= 30){
		units = 0x30 + unitsU;
	}
	else if (units >= 20){
		units = 0x20 + unitsU;
	}
	else if (units >= 10){
		units = 0x10 + unitsU;
	}
	return units;
}