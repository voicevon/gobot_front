//https://www.electronicshub.org/wp-content/uploads/2021/02/ESP32-Pinout-1.jpg


#include "i2c_commu.h"
#include "MyLibs/MyFunctions.hpp" 
#include "von/cpp/mqtt/task_mqtt.h"
#include "von/cpp/utility/logger.h"
#include "WString.h"

#include "../select_app.h"
#ifdef I_AM_ACUPUCTURE_MAIN_2022

I2c_commu obj_i2c_bus = I2c_commu();

// #define FORCE_ONLINE_CELL_COUNT 2
// int force_online_cell_list[FORCE_ONLINE_CELL_COUNT] = {3,4};
// #define FORCE_ONLINE_CELL_COUNT 11
// int force_online_cell_list[FORCE_ONLINE_CELL_COUNT] = {3, 4, 5, 6, 7, 8, 9, 16, 17, 19, 20};
#define FORCE_ONLINE_CELL_COUNT 6
int force_online_cell_list[FORCE_ONLINE_CELL_COUNT] = { 3,4,5,6,7,8 };

void init_online_cells(){
    //All online cells will never turn itself off.
    // For test_jig, The online_cells list should be empty.


    for (int i=0; i < FORCE_ONLINE_CELL_COUNT; i++){
       TouchCell* pCell = &obj_i2c_bus.Cells[i];
        // for (int j=0; j< FORCE_ONLINE_CELL_COUNT; j++){
            // if (force_online_cell_list[j] == pCell->Address){
        pCell->IsForceOnline = true;
        pCell->Address = force_online_cell_list[i];
        Serial.print("setting force-Online: ");
        Serial.println(pCell->Address);
    }
}

void publish_online_cells(){
    String payload="";
    for(int i=0; i<FORCE_ONLINE_CELL_COUNT;i++){
        TouchCell* pCell = &obj_i2c_bus.Cells[i];
        if(pCell->IsOnline){
            payload.concat(String(pCell->Address));
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
       TouchCell* pCell = &obj_i2c_bus.Cells[i];
        if (pCell->IsForceOnline){
            if(!pCell->IsOnline){
                Serial.print("This cell should be online, but Not  ");
                Serial.println(pCell->Address);
                payload.concat(String(pCell->Address));
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
    setup_wifi_mqtt_blocking_mode();
    while (!mqtt_is_connected){
        delay(1000);
        Serial.print(". ");
    }
    obj_i2c_bus.Init(CELL_ID_MIN, FORCE_ONLINE_CELL_COUNT);
    Logger::Info("setup is done....");
    init_online_cells();

}

String topic = "";
String payload ="";

void loop() {
    obj_i2c_bus.SpinOnce();
    // publish_online_cells();
    publish_error_cells();

    for(int i = 0; i< FORCE_ONLINE_CELL_COUNT; i++){
        TouchCell* cell = &obj_i2c_bus.Cells[i];
        if (cell->HasUpdate()){
            Logger::Debug("loop()  found updated cell");
            Logger::Print("HasUpdated()  cell address ", cell->Address);
            // Touch pin changed.  on->off  or  off->on
            for (int j=0; j<16; j++){
                if (cell->IsBitUpdated(j)){
                    topic = "actp/";
                    topic.concat(BODY_ID);
                    topic.concat("/");
                    topic.concat(cell->GetName(j));
                    payload = cell->GetMqttPayload(j);
                    Logger::Print("Mqtt topic", topic.c_str());
                    Logger::Print("Mqtt Payload", payload.c_str());
                    g_mqttClient.publish(topic.c_str(), 2, true, payload.c_str());
                }
            }
            cell->CopyCurrentToLast();
        }
    }  
}
#endif