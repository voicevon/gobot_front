#include "all_devices.h"
#ifdef I_AM_ROBOT_ASRS_AGV


#include "MyLibs/MyFunctions.hpp"
#include "bot_asrs_agv_core_yz.h"
#include "IoT/main_mqtt.h"
// #include "MyLibs/board_static.h"

StepControl objStepControl;  // This object can not inside any object?
BoardAllInOne board = BoardAllInOne();
BotAsrsAgvCoreYZ garment_robot = BotAsrsAgvCoreYZ(ROBOT_SERIAL_ID);
MessageQueue garment_bot_message_queue = MessageQueue();


void setup(){
    board.Init();
    garment_robot.InitAllinOne(&board, &objStepControl);

   // mqtt, bridge, receiver.
    setup_mqtt_block_connect();
    String mqtt_topic = "puma/bot/xROBOT_SERIAL_ID";
    mqtt_topic.replace("ROBOT_SERIAL_ID", String(ROBOT_SERIAL_ID));
    append_mqtt_bridge(mqtt_topic.c_str(), &garment_bot_message_queue, &garment_robot); 
    setup_mqtt_on_message_receive(); 

    Serial.println ("\n[Info] main.cpp  setup() is done. ------------------------------------  ");
    board.RepportRamUsage();
}

int64_t last_report_timestamp = 0;

void mqtt_report_states(){
    String topic = "puma/bot/states";
    uint8_t payload[60]; 
    garment_robot.GetMqtt_PubPayload(&payload[0]);
    const char * pp = (const char*)(&payload[0]);
    mqttClient.publish(topic.c_str(), 2, true, pp);
}

void loop(){
    garment_robot.SpinOnce();
    loop_mqtt();
    int64_t time_since_boot = esp_timer_get_time();
    if (time_since_boot - last_report_timestamp  > 30000000){  // 30 second
        mqtt_report_states();
        last_report_timestamp = time_since_boot;        
    }
}

#endif