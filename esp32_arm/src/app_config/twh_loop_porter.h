#pragma once

#define MY_TWH_ID 221109    // use replace to edit.

#define MY_ROBOT_ROW_ID 0   // Range is 0,1,2,3
 


// DO NOT MODIFY BELOW !!! 
#if MY_ROBOT_ROW_ID == 0
    #define MQTT_TOPIC_GCODE "wh221109/porter0/gcode"   
    #define MQTT_TOPIC_M408_REPORT_STATE_ON_SETUP "M408wh221109/porter0/state"
    #define MQTT_TOPIC_FOR_HOME_POSITION  "wh221109/porter0/home_position"
    #define MQTT_TOPIC_FOR_LEDS "wh221109/porter0/leds"
#endif

#if MY_ROBOT_ROW_ID == 1
    #define MQTT_TOPIC_GCODE "wh221109/porter1/gcode"   
    #define MQTT_TOPIC_M408_REPORT_STATE_ON_SETUP "M408wh221109/porter1/state"
    #define MQTT_TOPIC_FOR_HOME_POSITION  "wh221109/porter1/home_position"
    #define MQTT_TOPIC_FOR_LEDS "wh221109/porter1/leds"
#endif

#if MY_ROBOT_ROW_ID == 2
    #define MQTT_TOPIC_GCODE "wh221109/porter2/gcode"   
    #define MQTT_TOPIC_M408_REPORT_STATE_ON_SETUP "M408wh/221109/porter2/state"
    #define MQTT_TOPIC_FOR_HOME_POSITION  "wh221109/porter2/home_position"
    #define MQTT_TOPIC_FOR_LEDS "wh221109/porter2/leds"
#endif

#if MY_ROBOT_ROW_ID == 3
    #define MQTT_TOPIC_GCODE "wh221109/porter3/gcode"   
    #define MQTT_TOPIC_M408_REPORT_STATE_ON_SETUP "M408wh221109/porter3/state"
    #define MQTT_TOPIC_FOR_HOME_POSITION  "wh221109/porter3/home_position"
    #define MQTT_TOPIC_FOR_LEDS "wh221109/porter3/leds"
#endif

