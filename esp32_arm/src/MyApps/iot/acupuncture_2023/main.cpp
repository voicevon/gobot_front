//https://www.electronicshub.org/wp-content/uploads/2021/02/ESP32-Pinout-1.jpg

#include "touchpad_node.h"
#include "board/board.h"
#include "Mylibs/basic/logger.h"
#include "MyLibs/MyFunctions.hpp" 
#include "MyLibs/basic/memory_helper.h"
#include "Mqtt/wifi_mqtt_client.h"
// #include "Mqtt/mqtt_subscriber_base.h"
#include "Mqtt/mqtt_subscriber_manager.h"
#include "concern_sensor_setter.h"
// #include "ArduinoJson.h"
#include "WString.h"



#include "all_applications.h"
#ifdef I_AM_ACUPUCTURE_MAIN_2023
#define ACUPUCTURE_BODY_ID 101
#define NODES_COUNT_IN_THEORY 30
#define LED_PIN 2

AcupunctureBoard_2023 board;
TouchPad_Node all_touchpad_nodes[NODES_COUNT_IN_THEORY];
// payload is "147"  where 14 is node_id,  7 is channel_id.  147 = 14* node_id + channel_id
String __Mqtt_topic_of_monitor_sensor="acpt/monitor/sensor";   //  not use?
ConcernSensorSetter monitoring_sensor_command;
enum{
    STATE_IDLE = 1,
    STATE_ALL_NODES_ARE_ONLINE = 2,
    STATE_WORKING = 3,
    STATE_ALL_NODES_ARE_OFFLINE = 4,

}__State;

//    C = Connected -> Online
//    D = Died --> Offline
//    I = Installed, Unknown state ?? 
//    U = Uninstalled,
//    31 nodes
// char __NodeState[] = "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCC";  
//    421 Channels 
// char __Sensor_States[] ="FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF";
String node_states ="";
String sensors_state ="";
String previous_node_states = "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCC";
String previous_channel_states ="FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF";

bool IsInstalledNode(uint8_t node_id){
    #define INSTALLED_NODE_COUNT 2
    int installed_nodes[INSTALLED_NODE_COUNT] = {3, 4};
    
    for(int i=0; i< INSTALLED_NODE_COUNT; i++){
        if (node_id == installed_nodes[i])
            return true;
    }
    return false;
}

void Init_All_Touchpad_Nodes(bool all_nodes_in_theory){
    for(int i=0; i< NODES_COUNT_IN_THEORY; i++){
        TouchPad_Node* node = &all_touchpad_nodes[i];
        bool is_installed_node = IsInstalledNode(i) || all_nodes_in_theory;
        node->Init( i, is_installed_node); 
    }
}

void setup() {
    delay(2000);
    board.Init();
    Init_All_Touchpad_Nodes(false);

    setup_wifi_mqtt_blocking_mode();
    Logger::Print("setup() waiting mqtt_connectiong.",1);
    // while (!mqtt_is_connected){
    //     delay(100);
    // }
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);
    __State = STATE_IDLE;
    // gs_MqttSubscriberManager::Instance().AddSubscriber("wh221109/placing_leds", &monitoring_sensor_command);

    Logger::Info("Setup is done.");
}


// There are three mqtt topics:
// 1. acpt/001/node  nodes state  in [not installed,  offline,  online]
// 2. acpt/001/channel  Channels state of cell, in [not installed, died, touch_on, touch_off]
// 3. acpt/001/sensor_value   monitored sensor value, you can draw a chart from the data.
void publish_states(int body_id){

    // bool has_updated = false;

    // String node_states ="";
    // String sensors_state ="";
    // // Logger::Debug("mqtt_publish");
    // for(int i=0; i<NODES_COUNT_IN_THEORY; i++){
    //     TouchPad_Node* node = &all_touchpad_nodes[i];
    //     node_states.concat(node->GetState());
    //     sensors_state.concat(node->GetSensorsState());
    // }
    String topic_of_nodes_state = "acpt/" + String(body_id) + "/nodes" ;

    int mismatch_at =  MemoryHelper::Find_MismachLocation(previous_node_states.c_str(), node_states.c_str(), node_states.length());
    if (mismatch_at >=0 ){
        memcpy((uint8_t*)(previous_node_states.c_str()), node_states.c_str(), mismatch_at + 1);  // only this works.
        //          this does not work, don't delete.
        //          memcpy(&previous_node_states, &node_states, mismatch_at + 1);  
        // Serial.println("after memcpy");
        // Serial.println(previous_node_states);
        g_mqttClient.publish(topic_of_nodes_state.c_str(), 2, true, node_states.c_str()); 
    }

    String topic_channels = "acpt/" + String(body_id) + "/channels";
    mismatch_at = MemoryHelper::Find_MismachLocation(sensors_state.c_str(), previous_channel_states.c_str(), sensors_state.length());
    if (mismatch_at >=0){
        bool debug = true;
        if (debug){
            Logger::Info("channel_state updated:  node_id, sensor_id ---- value, state");
            // Logger::Print("changed sensor_number", mismatch_at);
            int node_id = mismatch_at / 14;
            int sensor_id = mismatch_at  % 14;
            int sensor_value = all_touchpad_nodes[node_id].GetSingleSensor(sensor_id)->GetSensorValue();
            String sensor_state = all_touchpad_nodes[node_id].GetSingleSensor(sensor_id)->GetState();
            Logger::Print(String(node_id).c_str(), String(sensor_id).c_str());
            Logger::Print(String(sensor_value).c_str(), String(sensor_state).c_str() );
        }

        memcpy((uint8_t *)(previous_channel_states.c_str()), sensors_state.c_str(), mismatch_at + 1);
        g_mqttClient.publish(topic_channels.c_str(), 2, true, sensors_state.c_str());
    }

  

}

void Publish_ConcerndSensor(int body_id){
    static int last_sent_sensor_value = 0;
    // Monitor a certain sesor.
    // int sensor_number = atoi(monitor.Get());   cause an exception !!!
    // String str_node_sensor = String(monitoring_sensor_command.Get());

    int sensor_number = 14*12 + 7;
    int node_id = sensor_number / 14;
    int sensor_index = sensor_number % 14;
    String topic_sensor_value = "acpt/" + String(body_id) + "/sensor_value" ;   // monitor sensor value

    uint8_t sensor_value = all_touchpad_nodes[node_id].GetSingleSensor(sensor_index)->GetSensorValue();
    if (abs(last_sent_sensor_value - sensor_value ) > 10){
        last_sent_sensor_value = sensor_value;
        // Logger::Print("payload sensor value", sensor_value);
        g_mqttClient.publish(topic_sensor_value.c_str(), 2, true, String(sensor_value).c_str());
    }
}
void loop() {

    if (__State == STATE_IDLE){
        __State = STATE_ALL_NODES_ARE_ONLINE;
    }
    if (__State == STATE_ALL_NODES_ARE_ONLINE){
        __State = STATE_WORKING;
    }
    if (__State == STATE_WORKING){
        for(int i = 0; i< NODES_COUNT_IN_THEORY; i++){
            // update sensor value,  review the received data.
            if (IsInstalledNode(i)){
                TouchPad_Node* node = &all_touchpad_nodes[i];  
                board.GetI2C_Master()->ReadSlaveNode(node);
                node->Process_RxBuffer();  
                node_states.concat(node->GetState());
                sensors_state.concat(node->GetSensorsState());
            }
        }
        publish_states(ACUPUCTURE_BODY_ID);
        if (node_states.indexOf('C') == -1){
            __State = STATE_ALL_NODES_ARE_OFFLINE;
        }
    }
    if (__State == STATE_ALL_NODES_ARE_OFFLINE){
        // bool all_is_offline = true;    if (all_is_offline) {
        Logger::Debug("loop(),  all nodes are offline, reseting all nodes , even those are not installed." );
        Init_All_Touchpad_Nodes(true);   // TODO:  try more times before set offline.
        delay(200);

        // digitalWrite(LED_PIN, LOW);
        // if (all_is_offline){
        //     digitalWrite(LED_PIN, HIGH);
        // } else {
        //     is_online_checking = false;
        // }
    }
    int concern_sensor_number = *(monitoring_sensor_command.Get());
    Publish_ConcerndSensor(ACUPUCTURE_BODY_ID);
}  
#endif