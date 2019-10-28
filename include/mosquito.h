#ifndef _MOSQUITO_H
#define _MOSQUITO_H

#include <iostream>
#include <cstdlib>
#include <string>

#include <mosquittopp.h>

#define SUBSCRIBE_TOPIC "projb/sensors/alarmthreshold/#"
#define PUBLISH_TOPIC "projb/sensors"
#define PUBLISH_TOPIC_HUMID "projb/sensors/humidity"
#define PUBLISH_TOPIC_TEMP "projb/sensors/temperature"
#define PUBLISH_TOPIC_LIGHT "projb/sensors/light"
#define PUBLISH_TOPIC_TIME "projb/sensors/time"
#define PUBLISH_TOPIC_ALARM "projb/sensors/alarm"

class mosquito_wrapper : public mosqpp::mosquittopp
{
public:
	mosquito_wrapper(const char *id, const char *host, int port);
	~mosquito_wrapper(){}

	void on_connect(int rc);
	void on_message(const struct mosquitto_message *message);
	void on_subcribe(int mid, int qos_count, const int *granted_qos);

	void write_to_mqtt(float humid, int temp, int light, bool alarm_on, std::string time);
};

extern float MAX_THRESH;
extern float MIN_THRESH;

#endif
