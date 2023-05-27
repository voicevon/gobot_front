//https://www.electronicshub.org/wp-content/uploads/2021/02/ESP32-Pinout-1.jpg

#include "touchpad_node.h"
#include "board/board.h"
#include "Mylibs/basic/logger.h"
#include "MyLibs/MyFunctions.hpp" 
#include "MyLibs/basic/memory_helper.h"
#include "Mqtt/wifi_mqtt_client.h"
#include "Mqtt/mqtt_subscriber_manager.h"
#include "concern_sensor_setter.h"
#include "WString.h"



#include "all_applications.h"
#ifdef I_AM_ACUPUCTURE_MAIN_2023
#define ACUPUCTURE_BODY_ID 101
#define NODES_COUNT_IN_THEORY 30
#define LED_PIN 2

AcupunctureBoard_2023 board;
TouchPad_Node all_nodes[NODES_COUNT_IN_THEORY];
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
char nodes_state[] = "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCC";  
//    421 Channels 
char __Sensor_States[] ="FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF";

bool IsInstalledNode(uint8_t node_id){
    #define INSTALLED_NODE_COUNT 2
    int installed_nodes[INSTALLED_NODE_COUNT] = {3, 4};
    
    for(int i=0; i< INSTALLED_NODE_COUNT; i++){
        if (node_id == installed_nodes[i])
            return true;
    }
    return false;
}

void Init__Nodes(){
    for(int i=0; i< NODES_COUNT_IN_THEORY; i++){
        TouchPad_Node* node = &all_nodes[i];
        // bool is_installed_node = IsInstalledNode(i) || all_nodes_in_theory;
        bool is_installed = IsInstalledNode(i);
        nodes_state[i] = 'D';
        node->Init( i, is_installed); 
        // if (is_installed_node){
        //     nodes_state[i] = 'C';
        // }
    }
}

void setup() {
    delay(2000);
    board.Init();
    Init__Nodes();

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
void publish_node_state(){
    String topic_of_nodes_state = "acpt/" + String(ACUPUCTURE_BODY_ID) + "/nodes" ;
    bool nodes_state_is_updated = false;
    for (int n=0; n<NODES_COUNT_IN_THEORY; n++){
        if (IsInstalledNode(n)){
            if (nodes_state[n] != all_nodes[n].GetState()){
                nodes_state[n] = all_nodes[n].GetState();
                nodes_state_is_updated = true;
            }
        }
    }
    if (nodes_state_is_updated){
        g_mqttClient.publish(topic_of_nodes_state.c_str(), 2, true, nodes_state); 
    }
}

void check_sensors_state(){
    String topic_channels = "acpt/" + String(ACUPUCTURE_BODY_ID) + "/channels";
    int updated_sensor_index = -1;
    for(int n=0; n<NODES_COUNT_IN_THEORY; n++){
        if(IsInstalledNode(n)){
            int sensor_index = all_nodes[n].GetUpdatedSensor();
            if (sensor_index >=0){
                updated_sensor_index = n * 14 + sensor_index;
                break;
            }
        }
    }
    if (updated_sensor_index >=0){
        g_mqttClient.publish(topic_channels.c_str(), 2, true, __Sensor_States);
    }
}

void Publish_ConcerndSensor(int sensor_number){
    static int last_sent_sensor_value = 0;
    // Monitor a certain sesor.
    // int sensor_number = atoi(monitor.Get());   cause an exception !!!
    // String str_node_sensor = String(monitoring_sensor_command.Get());

    // int sensor_number = 14*12 + 7;
    int node_id = sensor_number / 14;
    int sensor_index = sensor_number % 14;
    String topic_sensor_value = "acpt/" + String(ACUPUCTURE_BODY_ID) + "/sensor_value" ;   // monitor sensor value

    uint8_t sensor_value = all_nodes[node_id].GetSingleSensor(sensor_index)->GetSensorValue();
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
        publish_node_state();
        __State = STATE_WORKING;
    }
    if (__State == STATE_WORKING){
        for(int n = 0; n< NODES_COUNT_IN_THEORY; n++){
            // update sensor value,  review the received data.
            if (IsInstalledNode(n)){
                TouchPad_Node* node = &all_nodes[n];  
                board.GetI2C_Master()->ReadSlaveNode(node);
                node->Process_RxBuffer();  
                nodes_state[n] = node->GetState();
            }
        }
        int mismatch_at = MemoryHelper::Find_MismachLocation(nodes_state, "C", NODES_COUNT_IN_THEORY);
        if (mismatch_at <0){
            __State = STATE_ALL_NODES_ARE_OFFLINE;
        }else{
            publish_node_state();
            check_sensors_state();
        }
    }
    if (__State == STATE_ALL_NODES_ARE_OFFLINE){
        Logger::Debug("loop(),  all nodes are offline, reseting all nodes , even those are not installed." );
        Init__Nodes();   // TODO:  try more times before set offline.

        digitalWrite(LED_PIN, LOW);
        delay(200);
    }
    int concern_sensor_number = *(monitoring_sensor_command.Get());
    Publish_ConcerndSensor(concern_sensor_number);
}  
#endif