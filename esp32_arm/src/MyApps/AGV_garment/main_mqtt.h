#pragma once

extern void dispatch_MqttConnected(bool sessionPresent);
void setup_mqtt(GcodeQueue* gcode_queue, MessageQueue* message_queue);
extern void loop_mqtt();

extern bool mqtt_is_connected;