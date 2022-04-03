#pragma once

#include "Robot/gcode_consumer.h"

void setup_mqtt_block_connect();
void append_mqtt_link(const char* topic, GcodeQueue* local_gcode_queue, GcodeConsumer* local_gcode_consumer);
void loop_mqtt();
