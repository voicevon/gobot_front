#pragma once

#include "IoT/mqtt_message_consumer.h"
void setup_mqtt_block_connect();
void append_mqtt_link(const char* topic, MessageQueue* local_gcode_queue, MqttMessageConsumer* local_gcode_consumer);
void loop_mqtt();
