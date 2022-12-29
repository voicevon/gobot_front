//https://www.electronicshub.org/wp-content/uploads/2021/02/ESP32-Pinout-1.jpg

#include "all_applications.h"
#ifdef I_AM_ACUPUCTURE_MAIN

#include "i2c_commu.h"
#include "MyLibs/MyFunctions.hpp" 
#include "IoT/wifi_mqtt_client.h"

#include <string>
#include <HardwareSerial.h>


I2c_commu obj_i2c_bus = I2c_commu();

void init_online_cells(){
    //All online cells will never turn itself off.
    // For test_jig, The online_cells list should be empty.

    #define ONLINE_CELL_COUNT 2
    int cell_list[ONLINE_CELL_COUNT] = {3, 4};

    for (int i=0; i < CELLS_COUNT; i++){
       TouchCell* pCell = &obj_i2c_bus.Cells[i];
        for (int j=0; j< ONLINE_CELL_COUNT; j++){
            if (cell_list[j] == pCell->Address){
                pCell->auto_offline = false;
            }
        }
    }
    String payload="";
    for(int i=0; i<CELLS_COUNT;i++){
        TouchCell* pCell = &obj_i2c_bus.Cells[i];
        if(pCell->IsOnI2cBus){
            payload.concat(String(pCell->Address));
        }
    }

    g_mqttClient.publish("acupucture/online/addrs", 2, false,  payload.c_str());
}

void setup() {
    Serial.begin(115200);
    Serial.println("\n Hello, I am the main controller of actupuncture.  Commu with I2C , MQTT\n\n");
    obj_i2c_bus.Init(CELL_ID_MIN, CELLS_COUNT);

    setup_wifi_mqtt();
    while (!mqtt_is_connected){
        delay(100);
    }
}

std::string topic = "actu/foot/yongquanxue"; 
std::string payload ="123456";

void loop() {
    obj_i2c_bus.SpinOnce();

    for(int i = 0; i< CELLS_COUNT; i++){
        TouchCell* cell = &obj_i2c_bus.Cells[i];
        if (cell->HasUpdate()){
            // Touch pin changed.  on->off  or  off->on
            topic = "actp/";
            topic.append(BODY_ID);
            topic.append("/");
            for (int j=0; j<14; j++){
                if (cell->IsBitUpdated(j)){
                    Serial.println(j);
                    Serial.print(cell->GetName(j));
                    topic.append(cell->GetName(j));
                    payload = cell->GetMqttPayload(j);
                    g_mqttClient.publish(topic.c_str(), 2, true, payload.c_str());
                    
                    Serial.print(topic.c_str());
                    Serial.print("  ");
                    Serial.print(payload.c_str());
                    Serial.print("  ");
                    Serial.print(cell->Address);
                    Serial.print("-");
                    Serial.print(j);
                    Serial.print("  ");

                }
            }
            // Serial.print("Topic=");
            // Serial.print(topic.c_str());
            // Serial.print("  payload");
            // Serial.print(payload.c_str());

            g_mqttClient.publish(topic.c_str(), 2, true, payload.c_str());
        }
    }  
}
#endif