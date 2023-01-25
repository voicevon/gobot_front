#pragma once

#include "IoT/mqtt_message_consumer.h"
void setup_mono_remote_queue_via_mqtt(const char* topic, MessageQueue* local_gcode_queue, MqttMessageConsumer* local_gcode_consumer);
void mono_remote_queue_spin_once();

#include "IoT/wifi_mqtt_client.h"

