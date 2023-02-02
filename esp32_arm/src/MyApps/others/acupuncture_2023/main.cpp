//https://www.electronicshub.org/wp-content/uploads/2021/02/ESP32-Pinout-1.jpg



#include "i2c_master.h"
#include "touchpad_node.h"
#include "touchpad_channel.h"

#include "WString.h"
#include "Mylibs/basic/logger.h"
#include "MyLibs/MyFunctions.hpp" 
#include "Mqtt/wifi_mqtt_client.h"


#include "all_applications.h"
#ifdef I_AM_ACUPUCTURE_MAIN_2023
#define ACUPUCTURE_BODY_ID 101
#define NODES_COUNT_IN_THEORY 30


I2C_Master i2c_master;
TouchPad_Node all_touchpad_nodes[NODES_COUNT_IN_THEORY];


bool is_installed_node(uint8_t node_id){
    // #define INSTALLED_NODE_COUNT 1
    // int installed_nodes[INSTALLED_NODE_COUNT] = { 9 };

    // #define INSTALLED_NODE_COUNT 2
    // int installed_nodes[INSTALLED_NODE_COUNT] = {19,17};

    #define INSTALLED_NODE_COUNT 11
    int installed_nodes[INSTALLED_NODE_COUNT] = {3, 4, 5, 6, 7, 8, 9, 16, 17, 19, 20};

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
    while (!mqtt_is_connected){
        delay(100);
    }

}


// There are two mqtt topics:
// 1.  acpt/001/node  nodes state  in [not installed,  offline,  online]
// 2.  acpt/001/channel  Channels state of cell, in [not installed, died, touch_on, touch_off]
void mqtt_publish(int body_id){
    String payload_nodes ="";
    String payload_channels ="";
    String payload_sensor_value ="";
    for(int i=0; i<NODES_COUNT_IN_THEORY; i++){
        TouchPad_Node* node = &all_touchpad_nodes[i];
        payload_nodes.concat(node->GetMqttPayloadString());
        payload_channels.concat(node->GetChannelsPayloadString());
    }
    String topic_nodes = "acpt/" + String(body_id) + "/nodes" ;
    String topic_channels = "acpt/" + String(body_id) + "/channels";
    String topic_sensor_value = "acpt/" + String(body_id) + "/sensor_value" ;   // monitor sensor value

    
    g_mqttClient.publish(topic_nodes.c_str(), 2, true, payload_nodes.c_str());
    g_mqttClient.publish(topic_channels.c_str(), 2, true, payload_channels.c_str());
    g_mqttClient.publish(topic_sensor_value.c_str(), 2, true, payload_sensor_value.c_str());

}

void loop() {
    for(int i = 0; i< NODES_COUNT_IN_THEORY; i++){
        // update sensor value,  review the received data.
        TouchPad_Node* node = &all_touchpad_nodes[i];   
        node->Read_via_I2C();
        node->Review_RxBuffer();  
    }
    mqtt_publish(ACUPUCTURE_BODY_ID);
}  
#endif