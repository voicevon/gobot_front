//https://www.electronicshub.org/wp-content/uploads/2021/02/ESP32-Pinout-1.jpg



#include "i2c_master.h"
#include "touchpad_node.h"
#include "touchpad_channel.h"

#include "WString.h"
#include "Mylibs/basic/logger.h"
#include "MyLibs/MyFunctions.hpp" 
#include "MyLibs/basic/memory_helper.h"

#include "Mqtt/wifi_mqtt_client.h"
#include "Mqtt/remote_var.h"


#include "all_applications.h"
#ifdef I_AM_ACUPUCTURE_MAIN_2023
#define ACUPUCTURE_BODY_ID 101
#define NODES_COUNT_IN_THEORY 30


I2C_Master i2c_master;
TouchPad_Node all_touchpad_nodes[NODES_COUNT_IN_THEORY];
String monitoring_sensor_topic="acpt/monitor/sensor";   // payload is "147"  where 14 is node_id,  7 is channel_id.  147 = 14* node_id + channel_id
RemoteVar_via_Mqtt monitor(monitoring_sensor_topic.c_str());
// int monitoring_sensor_number = 0;


bool is_installed_node(uint8_t node_id){
    // #define INSTALLED_NODE_COUNT 1
    // int installed_nodes[INSTALLED_NODE_COUNT] = { 18 };

    #define INSTALLED_NODE_COUNT 2
    int installed_nodes[INSTALLED_NODE_COUNT] = {11,12};

    // #define INSTALLED_NODE_COUNT 11
    // int installed_nodes[INSTALLED_NODE_COUNT] = {3, 4, 5, 6, 7, 8, 9, 16, 17, 19, 20};

    for(int i=0; i< INSTALLED_NODE_COUNT; i++){
        if (node_id == installed_nodes[i])
            return true;
    }
    return false;
}

void Init_All_Touchpad_Nodes(){
    for(int i=0; i< NODES_COUNT_IN_THEORY; i++){
        TouchPad_Node* node = &all_touchpad_nodes[i];
        node->Init(&i2c_master, i, is_installed_node(i)); 
    }
}

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("\n Hello, I am the main controller of actupuncture.  Commu with I2C , MQTT\n\n");
    i2c_master.Init();

    Init_All_Touchpad_Nodes();

    setup_wifi_mqtt();
    Logger::Print("setup() waiting mqtt_connectiong.",1);
    while (!mqtt_is_connected){
        delay(100);
    }
    Logger::Info("Setup is done.");
}


// bool is_same(const char* a, const char* b, int length){
//     for(int i=0; i<length; i++){
//         if (*(a+i) != *(b+i))
//             return false;
//     }
//     return true;
// }

// There are three mqtt topics:
// 1. acpt/001/node  nodes state  in [not installed,  offline,  online]
// 2. acpt/001/channel  Channels state of cell, in [not installed, died, touch_on, touch_off]
// 3. acpt/001/sensor_value   monitored sensor value, you can draw a chart from the data.
void mqtt_publish(int body_id){
    static String previous_node_states = "DDDDDDDDDDDDDDDDDDDDDDDDDDDDD";
    static String previous_channel_states ="DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD";

    // bool has_updated = false;
    static int last_sent_sensor_value = 0;

    String node_states ="";
    String channel_states ="";
    // Logger::Debug("mqtt_publish");
    for(int i=0; i<NODES_COUNT_IN_THEORY; i++){
        TouchPad_Node* node = &all_touchpad_nodes[i];
        node_states.concat(node->GetMqttPayloadString());
        channel_states.concat(node->GetChannelsPayloadString());
    }
    String topic_nodes = "acpt/" + String(body_id) + "/nodes" ;

    int delta_at =  MemoryHelper::CompareSame(previous_node_states.c_str(), node_states.c_str(), node_states.length());
    if (delta_at >=0 ){
        // Logger::Info("mqtt_publish()  nodes state");
        // Logger::Print("changed node ", delta_at);
        // Serial.print("compare length= ");
        // Serial.println(node_states.length());
        // Serial.println(node_states);
        // Serial.println(previous_node_states);
        memcpy((uint8_t*)(previous_node_states.c_str()), node_states.c_str(), delta_at + 1);  // only this works.
        //          this does not work, don't delete.
        //          memcpy(&previous_node_states, &node_states, delta_at + 1);  
        // Serial.println("after memcpy");
        // Serial.println(previous_node_states);
        g_mqttClient.publish(topic_nodes.c_str(), 2, true, node_states.c_str()); 
    }

    String topic_channels = "acpt/" + String(body_id) + "/channels";
    
    delta_at = MemoryHelper::CompareSame(channel_states.c_str(), previous_channel_states.c_str(), channel_states.length());
    if (delta_at >=0){
        Logger::Info("mqtt_publish()  channel_state updated");
        Logger::Print("changed sensor_number", delta_at);
        // Logger::Print("previous channel states", previous_channel_states.c_str());
        // Logger::Print("now channel states", channel_states.c_str());
        memcpy((uint8_t *)(previous_channel_states.c_str()), channel_states.c_str(), delta_at+1);
        // Serial.println("after memcpy");
        // Serial.println(previous_channel_states);
        g_mqttClient.publish(topic_channels.c_str(), 2, true, channel_states.c_str());
    }

  
    // Monitor a certain sensor.
    // int sensor_number = atoi(monitor.Get());   cause an exception !!!
    int sensor_number = 14*12 + 7;
    int node_id = sensor_number / 14;
    int channel_id = sensor_number % 14;
    String topic_sensor_value = "acpt/" + String(body_id) + "/sensor_value" ;   // monitor sensor value

    // String payload_sensor_value = String(all_touchpad_nodes[node_id].GetSensorValue(channel_id));
    uint8_t sensor_value = all_touchpad_nodes[node_id].GetSensorValue(channel_id);
    if (abs(last_sent_sensor_value - sensor_value ) > 10){
        last_sent_sensor_value = sensor_value;
        // Logger::Print("payload sensor value", sensor_value);
        g_mqttClient.publish(topic_sensor_value.c_str(), 2, true, String(sensor_value).c_str());
    }
}


void loop() {
    bool all_is_offline = true;
    for(int i = 0; i< NODES_COUNT_IN_THEORY; i++){
        // update sensor value,  review the received data.
        TouchPad_Node* node = &all_touchpad_nodes[i];   
        node->Read_via_I2C();
        node->Review_RxBuffer();  
        // All is offline, reset all nodes.
        if (node->Get_I2CSlaveNode()->GetState() == I2C_SlaveNode::EnumState::ONLINE_CONNECTED) all_is_offline = false;
    }
    if (all_is_offline) Init_All_Touchpad_Nodes();   // TODO:  try more times before set offline.
    mqtt_publish(ACUPUCTURE_BODY_ID);
    // Logger::Print("loop()  ponit",4);
    // delay(1000);
}  
#endif