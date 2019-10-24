#include "mosquito.h"

using namespace std;

mosquito_wrapper::mosquito_wrapper(const char *id, const char *host, int port) : mosquittopp(id)
{
	mosqpp::lib_init();			// Initialize libmosquitto
	int keepalive = 120;
	connect(host, port, keepalive);		// Connect to MQTT Broker
}

void mosquito_wrapper::on_connect(int rc)
{
	if(rc!=0)
	{
		cout << "Unable to connect to MQTT broker" << endl;
	}
	else
	{
		//successful connect, subscribe to topic
		subscribe(NULL, SUBSCRIBE_TOPIC);
	}
}

void mosquito_wrapper::on_subcribe(int mid, int qos_count, const int *granted_qos)
{
	cout << "Subscription successful. " << endl;
}

void mosquito_wrapper::on_message(const struct mosquitto_message *message)
{
	//we've recieved a message from the topic we subscibed to in on_connect()
}

void mosquito_wrapper::write_to_mqtt(float humid, int temp, int light, bool alarm_on)
{
	string humid_str = to_string(humid);
	string temp_str = to_string(temp);
	string light_str = to_string(light);
	string alarm_str = alarm_on ? "YES" : "NO";

	publish(NULL, PUBLISH_TOPIC_HUMID, humid_str.length(), humid_str.c_str());
	publish(NULL, PUBLISH_TOPIC_TEMP, temp_str.length(), temp_str.c_str());
	publish(NULL, PUBLISH_TOPIC_LIGHT, light_str.length(), light_str.c_str());
	publish(NULL, PUBLISH_TOPIC_ALARM, alarm_str.length(), alarm_str.c_str());
}
