#include "all_devices.h"
#ifdef I_AM_GARMENT_BOT


#include "MyLibs/MyFunctions.hpp"
#include "garment_bot.h"
#include "IoT/main_mqtt.h"

GarmentBot *garment_robot;
MessageQueue* garment_bot_message_queue;


void setup(){
    Serial.begin(115200);
    Serial.println("Hi there, I am your lovely bot,  Garmentbot, include AGV.  Keep smiling :)");
    garment_robot = new GarmentBot();
    garment_robot->Init();

   // mqtt, bridge, receiver.
    setup_mqtt_block_connect();
    garment_bot_message_queue = new MessageQueue();
    String mqtt_topic = "puma/xROBOT_SERIAL_ID/bot";
    mqtt_topic.replace("SERIAL_ID",ROBOT_SERIAL_ID);
    append_mqtt_bridge(mqtt_topic.c_str(), garment_bot_message_queue, garment_robot); 
    setup_mqtt_on_message_receive(); 

    Serial.println ("\n  main.cpp  setup() is done. ------------------------------------ \n");
}

int64_t last_report_timestamp = 0;

void mqtt_report_states(){
    String topic = "puma/bot/states";
    String payload = "id:1234, bv:13.2,st:1";
    mqttClient.publish(topic.c_str(), 2, true, payload.c_str());
}

void loop(){
    garment_robot->SpinOnce();
    loop_mqtt();
    int64_t time_since_boot = esp_timer_get_time();
    if (time_since_boot - last_report_timestamp  > 30000000){  // 30 second
        mqtt_report_states();
        last_report_timestamp = time_since_boot;        
    }
}

#endif