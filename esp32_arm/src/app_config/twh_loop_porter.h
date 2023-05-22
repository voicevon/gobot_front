#pragma once

#define MY_TWH_ID 221109    // use replace to edit.

#define MY_ROBOT_ROW_ID 1   // Range is 0,1,2,3
 


// DO NOT MODIFY BELOW !!! 
#if MY_ROBOT_ROW_ID == 0
    #define MQTT_TOPIC_GCODE "wh221109/r0/gcode"   
    #define MQTT_TOPIC_M408_REPORT_STATE_ON_SETUP "M408wh221109/r0/state"
    #define MQTT_TOPIC_FOR_HOME_POSITION  "wh221109/r0/home_position"
    #define MQTT_TOPIC_FOR_LEDS "wh221109/r0/leds"
#endif

#if MY_ROBOT_ROW_ID == 1
    #define MQTT_TOPIC_GCODE "wh221109/r1/gcode"   
    #define MQTT_TOPIC_M408_REPORT_STATE_ON_SETUP "M408wh221109/r1/state"
    #define MQTT_TOPIC_FOR_HOME_POSITION  "wh221109/r1/home_position"
    #define MQTT_TOPIC_FOR_LEDS "wh221109/r1/leds"
#endif

#if MY_ROBOT_ROW_ID == 2
    #define MQTT_TOPIC_GCODE "wh221109/r2/gcode"   
    #define MQTT_TOPIC_M408_REPORT_STATE_ON_SETUP "M408wh/221109/r2/state"
    #define MQTT_TOPIC_FOR_HOME_POSITION  "wh221109/r2/home_position"
    #define MQTT_TOPIC_FOR_LEDS "wh221109/r2/leds"
#endif

#if MY_ROBOT_ROW_ID == 3
    #define MQTT_TOPIC_GCODE "wh221109/r3/gcode"   
    #define MQTT_TOPIC_M408_REPORT_STATE_ON_SETUP "M408wh221109/r3/state"
    #define MQTT_TOPIC_FOR_HOME_POSITION  "wh221109/r3/home_position"
    #define MQTT_TOPIC_FOR_LEDS "wh221109/r3/leds"
#endif

