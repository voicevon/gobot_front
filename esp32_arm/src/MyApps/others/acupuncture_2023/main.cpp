//https://www.electronicshub.org/wp-content/uploads/2021/02/ESP32-Pinout-1.jpg



#include "i2c_master.h"
#include "MyLibs/MyFunctions.hpp" 
#include "Mqtt/wifi_mqtt_client.h"

#include "WString.h"
#include "Mylibs/basic/logger.h"
#include "touchpad_channel.h"

#include "all_applications.h"
#ifdef I_AM_ACUPUCTURE_MAIN_2023

I2C_Master i2c_master;

// #define FORCE_ONLINE_CELL_COUNT 2
// int force_online_cell_list[FORCE_ONLINE_CELL_COUNT] = {19,17};
// #define FORCE_ONLINE_CELL_COUNT 11
// int force_online_cell_list[FORCE_ONLINE_CELL_COUNT] = {3, 4, 5, 6, 7, 8, 9, 16, 17, 19, 20};
#define FORCE_ONLINE_CELL_COUNT 1
int force_online_cell_list[FORCE_ONLINE_CELL_COUNT] = { 9 };

// TouchPad_Channel all_channels[300];

TouchCell_2023 all_cells[20];

void Init_Cells(){
    all_cells[3].Init(3, 8);
    all_cells[4].Init(3, 8);
    all_cells[5].Init(3, 8);
    all_cells[6].Init(3, 8);
    all_cells[7].Init(3, 8);
}

void init_online_cells(){

    // uint8_t cell_id = 3;
    // uint8_t channel_id = 0;
    // all_channels[cell_id].Init(cell_id, channel_id, TouchPad_Channel::EnumState::WROKING);



    //All online cells will never turn itself off.
    // For test_jig, The online_cells list should be empty.


    for (int i=0; i < FORCE_ONLINE_CELL_COUNT; i++){
       TouchCell_2023* pCell = &i2c_master.Cells[i];
        // for (int j=0; j< FORCE_ONLINE_CELL_COUNT; j++){
            // if (force_online_cell_list[j] == pCell->Address){
        pCell->IsForceOnline = true;
        pCell->I2C_Address = force_online_cell_list[i];
        Serial.print("setting force-Online: ");
        Serial.println(pCell->I2C_Address);
    }
}

void publish_online_cells(){
    String payload="";
    for(int i=0; i<FORCE_ONLINE_CELL_COUNT;i++){
        TouchCell_2023* pCell = &i2c_master.Cells[i];
        if(pCell->IsOnline){
            payload.concat(String(pCell->I2C_Address));
            payload.concat(" ");
        }
    }
    Serial.println(payload);
    g_mqttClient.publish("acupucture/online/addrs", 2, true,  payload.c_str());
}

void publish_error_cells(){
    digitalWrite(2, LOW);
    bool has_error = false;
    String payload = "";
    for (int i=0; i < FORCE_ONLINE_CELL_COUNT; i++){
       TouchCell_2023* pCell = &i2c_master.Cells[i];
        if (pCell->IsForceOnline){
            if(!pCell->IsOnline){
                Serial.print("This cell should be online, but Not  ");
                Serial.println(pCell->I2C_Address);
                payload.concat(String(pCell->I2C_Address));
                payload.concat(" ");
                has_error = true;
                pCell->SetOnline();
            }
        }
    }
    if (has_error){
        digitalWrite(2, HIGH);
        Serial.println(payload);
        g_mqttClient.publish("acupucture/001/error/addrs", 2, true,  payload.c_str());
    }
}

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("\n Hello, I am the main controller of actupuncture.  Commu with I2C , MQTT\n\n");

    i2c_master.Init(CELL_ID_MIN, FORCE_ONLINE_CELL_COUNT);

    setup_wifi_mqtt();
    while (!mqtt_is_connected){
        delay(100);
    }
    init_online_cells();   // TODO:  remove this
    Init_Cells();

}

String topic = "";
String payload ="";

void loop() {

    publish_error_cells();

    for(int i = 0; i< FORCE_ONLINE_CELL_COUNT; i++){
        TouchCell_2023* cell = &i2c_master.Cells[i];   // all_cells? 
        i2c_master.ReadSingleCell(cell);
        cell->Review_RxBuffer();  
        // cell->GetMqttPayload()
         // Duing reading cell, the channels inside the cell will be updated.
        // We assume : there is only one channel could be changed.
        if (cell->HasUpdate()){
            Logger::Debug("loop()  found updated cell");
            Logger::Print("HasUpdated()  cell address ", cell->I2C_Address);
            // Touch pin changed.  on->off  or  off->on
            for (int j=0; j<16; j++){
                // if (cell->IsBitUpdated(j)){
                    topic = "actp/";
                    topic.concat(BODY_ID);
                    topic.concat("/");
                    topic.concat(cell->GetName(j));
                    payload = cell->GetMqttPayload(j);
                    Logger::Print("Mqtt topic", topic.c_str());
                    Logger::Print("Mqtt Payload", payload.c_str());
                    g_mqttClient.publish(topic.c_str(), 2, true, payload.c_str());
                // }
            }
            cell->CopyCurrentToLast();
        }
    }  
}
#endif