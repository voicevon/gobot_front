#include "../select_app.h"
// #include "all_devices.h"
#ifdef I_AM_GARMENT_BOT


#include "MyLibs/MyFunctions.hpp"
#include "garment_bot.h"

// #include "Mqtt/main_mqtt.h"

GarmentBot *garment_robot;
MessageQueue* garment_bot_message_queue;


void setup(){
    Serial.begin(115200);
    Serial.println("Hi there, I am your lovely bot,  Garmentbot, include AGV.  Keep smiling :)");
    garment_robot = new GarmentBot(ROBOT_SERIAL_ID);
    garment_robot->Init();

   // mqtt, bridge, receiver.
    // connect_to_mqtt_broker();
    garment_bot_message_queue = new MessageQueue();
    String mqtt_topic = "puma/bot/xROBOT_SERIAL_ID";
    mqtt_topic.replace("ROBOT_SERIAL_ID", String(ROBOT_SERIAL_ID));
    mono_remote_queue_bridge_via_mqtt_setup(mqtt_topic.c_str(), garment_bot_message_queue, garment_robot); 
    // setup_mqtt_on_message_receive(); 

    Serial.println ("\n  [Info] Garment bot setup() is done. ------------------------------------ \n");
}

int64_t last_report_timestamp = 0;

void mqtt_report_states(){
    String topic = "puma/bot/states";
    uint8_t payload[60]; 
    garment_robot->GetMqtt_PubPayload(&payload[0]);
    const char * pp = (const char*)(&payload[0]);
    g_mqttClient.publish(topic.c_str(), 2, true, pp);
}

void loop(){
    garment_robot->SpinOnce();
    mono_remote_queue_bridge_spin_once();
    int64_t time_since_boot = esp_timer_get_time();
    if (time_since_boot - last_report_timestamp  > 30000000){  // 30 second
        mqtt_report_states();
        last_report_timestamp = time_since_boot;        
    }
}

#endif