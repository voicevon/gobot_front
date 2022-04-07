
#pragma once

#include <AsyncMqttClient.h>

void setup_wifi_mqtt();

extern AsyncMqttClient mqttClient;
extern bool mqtt_is_connected;

