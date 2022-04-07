//https://www.electronicshub.org/wp-content/uploads/2021/02/ESP32-Pinout-1.jpg

#include "all_devices.h"
#ifdef I_AM_ACUPUCTURE_MAIN_ESP32

#include "i2c_commu.h"
// #include "MyLibs/mqtt/wifi_mqtt_client.h"
#include "MyLibs/MyFunctions.hpp" 

#include <string>
#include <HardwareSerial.h>


I2c_commu obj_i2c_bus = I2c_commu();
// Wifi_MqttClient obj_mqtt = Wifi_MqttClient();

//********************************************************************************************
//    MQTT publish only, will not subscribe and topic
//********************************************************************************************
// #include "IoT/mqtt_syncer.h"
#include "IoT/wifi_mqtt_client.h"

// extern AsyncMqttClient mqttClient;
// extern bool mqtt_is_connected;
// void dispatch_MqttConnected(bool sessionPresent){
//     mqtt_is_connected = true;
//     Serial.println("MQTT is connected!");
// }
// void dispatch_MqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) {
//     // Will never received and message.
// }
// void Begin_WifiRabbitMqtt(){
//     setup_wifi_mqtt();
//     // mqttClient.onConnect(dispatch_MqttConnected);

// }
//*********************  end of MQTT  *******************************************

void setup() {
    Serial.begin(115200);
    Serial.println("\n Hello, I am the main controller of actupuncture.  Commu with I2C , MQTT\n\n");
    obj_i2c_bus.Init();

    // Begin_WifiRabbitMqtt
    setup_wifi_mqtt();
    while (!mqtt_is_connected){
        delay(100);
    }
}

std::string topic = "actu/foot/yongquanxue"; 
std::string payload ="123456";

void loop() {

    obj_i2c_bus.SpinOnce();

    for(int i = 0; i<CELLS; i++){
        TouchCell* cell = &obj_i2c_bus.Cells[i];
        if (cell->HasUpdate()){
            // Touch pin changed
            topic = "actp/";
            topic.append(BODY_ID);
            topic.append("/");
            for (int j=0; j<14; j++){
                if (cell->IsBitUpdated(j)){
                    Serial.println(j);
                    Serial.print(cell->GetName(j));
                    topic.append(cell->GetName(j));
                    payload = cell->GetMqttPayload(j);
                    mqttClient.publish(topic.c_str(), 2, true, payload.c_str());
                    
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

            mqttClient.publish(topic.c_str(), 2, true, payload.c_str());
        }
    }  
}
#endif