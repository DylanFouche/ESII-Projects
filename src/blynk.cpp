#include "blynk.h"

//setting up BLYNK
#define BLYNK_PRINT stdout

#include <wiringPi.h>
#include <BlynkApiWiringPi.h>
#include <BlynkSocket.h>
#include <BlynkOptionsParser.h>

static BlynkTransportSocket _blynkTransport;
BlynkSocket Blynk(_blynkTransport);
#include <BlynkWidgets.h>

char AUTH_TOKEN[] = "pZn612Bmfqu9_3344QW0NE6sAWEkkj94";
char system_time[8];

//define BLYNK virtual pins
#define SYSTEM_TIME V1
#define HUMIDITY V2
#define TEMP V3
#define LIGHT V4
#define ALARM V5

void setup_blynk(void)
{
    Blynk.begin(AUTH_TOKEN);
    while (Blynk.connect() == false);
}

void write_to_blynk(float humid, int temp, int light, bool alarm_on)
{
    if (Blynk.connected()) {
        Blynk.run();
        Blynk.virtualWrite(SYSTEM_TIME, system_time);
        Blynk.virtualWrite(HUMIDITY, humid);
        Blynk.virtualWrite(TEMP, temp);
        Blynk.virtualWrite(LIGHT, light);
        Blynk.virtualWrite(ALARM, alarm_on);
    }
}

void update_blynk_time(int HH, int MM, int SS)
{
    //update system_time string for blynk
    char HHstr[2];
    char MMstr[2];
    char SSstr[2];
    sprintf(HHstr, "%d", HH);
    sprintf(MMstr, "%d", MM);
    sprintf(SSstr, "%d", SS);
    strcpy(system_time, HHstr);
    strcat(system_time,":");
    strcat(system_time, MMstr);
    strcat(system_time,":");
    strcat(system_time, SSstr);
}