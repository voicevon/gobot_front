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

#define CELLS_COUNT_IN_THEORY 30

I2C_Master i2c_master;
TouchPad_Node all_touchpad_nodes[CELLS_COUNT_IN_THEORY];


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
    for(int i=0; i< CELLS_COUNT_IN_THEORY; i++){
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

String topic = "";
String payload ="";


// There are two mqtt topics:
// 1.  acpt/001/node  nodes state  in [not installed,  offline,  online]
// 2.  acpt/001/channel  Channels state of cell, in [not installed, died, touch_on, touch_off]
void loop() {
    for(int i = 0; i< CELLS_COUNT_IN_THEORY; i++){
        // update sensor value,  review the received data.
        TouchPad_Node* node = &all_touchpad_nodes[i];   
        node->Read_via_I2C();
        node->Review_RxBuffer();  

        // if there is any update of the channel, cell,  publish via MQTT.

        
        // cell->GetMqttPayload()
         // Duing reading cell, the channels inside the cell will be updated.
        // We assume : there is only one channel could be changed.
        // if (cell->HasUpdate()){
            Logger::Debug("loop()  found updated cell");
            // Logger::Print("HasUpdated()  cell address ", node);
            // Touch pin changed.  on->off  or  off->on
            for (int j=0; j<16; j++){
                // if (cell->IsBitUpdated(j)){
                    topic = "actp/";
                    topic.concat(BODY_ID);
                    topic.concat("/");
                    // topic.concat(cell->GetName(j));
                    // payload = cell->GetMqttPayload(j);
                    Logger::Print("Mqtt topic", topic.c_str());
                    Logger::Print("Mqtt Payload", payload.c_str());
                    g_mqttClient.publish(topic.c_str(), 2, true, payload.c_str());
                // }
            }
            // cell->CopyCurrentToLast();
        }
}  
#endif