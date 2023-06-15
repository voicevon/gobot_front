#include <AsyncMqttClient.h>

void setup_wifi_mqtt_blocking_mode();

extern AsyncMqttClient g_mqttClient;  //TODO:  be a static method of a class
extern bool mqtt_is_connected;

extern void TaskMqtt(void* parameter);
