#pragma once

#include "IoT/mqtt_message_consumer.h"
void setup_mqtt_block_connect();
void append_mqtt_bridge(const char* topic, MessageQueue* local_gcode_queue, MqttMessageConsumer* local_gcode_consumer);
void setup_mqtt_on_message_receive();
void loop_mqtt();

#include "IoT/wifi_mqtt_client.h"

