#pragma once

#define MY_TWH_ID 221109
#define MY_ROBOT_ROW_ID 0   //Range is 0,1,2,3

 
#if MY_ROBOT_ROW_ID == 0
    #define MQTT_TOPIC_GCODE "twh/221109/r0/gcode"   
    #define MQTT_TOPIC_M408_REPORT_STATE_ON_SETUP "M408twh/221109/r0/state"
    #define MQTT_TOPIC_FOR_HOME_POSITION  "twh/221109/r0/home_position"
#endif

#if MY_ROBOT_ROW_ID == 1
    #define MQTT_TOPIC_GCODE "twh/221109/r1/gcode"   
    #define MQTT_TOPIC_M408_REPORT_STATE_ON_SETUP "M408twh/221109/r1/state"
    #define MQTT_TOPIC_FOR_HOME_POSITION  "twh/221109/r1/home_position"
#endif

#if MY_ROBOT_ROW_ID == 2
    #define MQTT_TOPIC_GCODE "twh/221109/r2/gcode"   
    #define MQTT_TOPIC_M408_REPORT_STATE_ON_SETUP "M408twh/221109/r2/state"
    #define MQTT_TOPIC_FOR_HOME_POSITION  "twh/221109/r2/home_position"
#endif

#if MY_ROBOT_ROW_ID == 3
    #define MQTT_TOPIC_GCODE "twh/221109/r3/gcode"   
    #define MQTT_TOPIC_M408_REPORT_STATE_ON_SETUP "M408twh/221109/r3/state"
    #define MQTT_TOPIC_FOR_HOME_POSITION  "twh/221109/r3/home_position"
#endif

