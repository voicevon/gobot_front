#pragma once

#include "IoT/mqtt_message_consumer.h"
// void connect_to_mqtt_broker();
void setup_mono_remote_queue_via_mqtt(const char* topic, MessageQueue* local_gcode_queue, MqttMessageConsumer* local_gcode_consumer);
// void setup_mqtt_on_message_receive();
void loop_mqtt();

#include "IoT/wifi_mqtt_client.h"

