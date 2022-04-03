#pragma once

#include "Robot/gcode_consumer.h"

void setup_mqtt_block_connect();
void append_mqtt_link(const char* topic, GcodeQueue* local_gcode_queue, GcodeConsumer* local_gcode_consumer);

// void mqtt_box_mover_link_gcode_queue(const char* mqtt_topic, GcodeQueue* gcode_queue);
// void mqtt_agv_link_message_queue(const char* mqtt_topic, MessageQueue* message_queue);

void loop_mqtt();
