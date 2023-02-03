//https://www.electronicshub.org/wp-content/uploads/2021/02/ESP32-Pinout-1.jpg



#include "i2c_master.h"
#include "touchpad_node.h"
#include "touchpad_channel.h"

#include "WString.h"
#include "Mylibs/basic/logger.h"
#include "MyLibs/MyFunctions.hpp" 
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
    #define INSTALLED_NODE_COUNT 1
    int installed_nodes[INSTALLED_NODE_COUNT] = { 18 };

    // #define INSTALLED_NODE_COUNT 2
    // int installed_nodes[INSTALLED_NODE_COUNT] = {19,17};

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




// There are three mqtt topics:
// 1. acpt/001/node  nodes state  in [not installed,  offline,  online]
// 2. acpt/001/channel  Channels state of cell, in [not installed, died, touch_on, touch_off]
// 3. acpt/001/sensor_value   monitored sensor value, you can draw a chart from the data.
void mqtt_publish(int body_id){
    String payload_nodes ="";
    String payload_channels ="";
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


    // Monitor a certain sensor.
    int sensor_number = atoi(monitor.Get());
    int node_id = sensor_number / 14;
    int channel_id = sensor_number % 14;
    String payload_sensor_value = String(all_touchpad_nodes[node_id].GetSensorValue(channel_id));
    g_mqttClient.publish(topic_sensor_value.c_str(), 2, true, payload_sensor_value.c_str());

}

void loop() {
    for(int i = 0; i< NODES_COUNT_IN_THEORY; i++){
        // update sensor value,  review the received data.
        TouchPad_Node* node = &all_touchpad_nodes[i];   
        Logger::Print("loop()  ponit",1);
        node->Read_via_I2C();
        Logger::Print("loop()  ponit",2);
        node->Review_RxBuffer();  
        Logger::Print("loop()  ponit",3);
    }
    mqtt_publish(ACUPUCTURE_BODY_ID);
    Logger::Print("loop()  ponit",4);
}  
#endif