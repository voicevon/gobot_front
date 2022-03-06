#include "all_devices.h"
#ifdef I_AM_GARMENT_BOT

#include "Iot/wifi_mqtt_client.h"

#include "MyLibs/MyFunctions.hpp"
#include "garment_bot.h"
#include <WiFi.h>

// #include "Robot/Commu/commu_esp_now.h"



#define PIN_ENCODER_A 12
#define PIN_ENCODER_B 13

GarmentBot *mybot; // = GarmentBot();
// CommuEspNow_Master commu_esp_now;
// IrEncoder irEncoder_leftWheel = IrEncoder(PIN_ENCODER_A, PIN_ENCODER_B, 40);
// IrEncoderHelper irEncoderHelper = IrEncoderHelper();
// void doB(){irEncoder_leftWheel.handleB();}



//********************************************************************************************
//    MQTT and RabbitMQ
//********************************************************************************************
#include "Robot/command_queue_rabbit.h"
CommandQueueRabbit* commandQueueRabbit;
extern AsyncMqttClient mqttClient;
void dispatch_MqttConnected(bool sessionPresent){
    commandQueueRabbit->SubscribeMqtt(&mqttClient, "gobot/x2134/house", "gobot/x2134/house/fb");
}
void dispatch_MqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) {
    bool debug = false;
    if(debug){
        Serial.println("Publish received.");
        Serial.print("  topic: ");
        Serial.println(topic);
        Serial.print("  paylod: ");
        Serial.println(payload);
        Serial.print("  qos: ");
        Serial.println(properties.qos);
        Serial.print("  dup: ");
        Serial.println(properties.dup);
        Serial.print("  retain: ");
        Serial.println(properties.retain);
        Serial.print("  len: ");
        Serial.println(len);
        Serial.print("  index: ");
        Serial.println(index);
        Serial.print("  total: ");
        Serial.println(total);
    }
    commandQueueRabbit->OnReceived(payload, len);
}
void Begin_WifiRabbitMqtt(){
    setup_wifi_mqtt();
    commandQueueRabbit = new CommandQueueRabbit();
    commandQueueRabbit->LinkLocalCommandQueue(mybot->objBoxMover.GetCommandQueue());
    mqttClient.onConnect(dispatch_MqttConnected);
    mqttClient.onMessage(dispatch_MqttMessage);
}
//*********************  end of MQTT and RabbitMQ  *******************************************

// void app_mqtt_subscribe(){
//     mqttClient.subscribe("garmentbot/mode", 2);
//     Serial.println("Subscribed garmentbot/*");
// }

// void app_mqtt_received_message( char* topic, char* payload){
//     const char * cc = (const char*)(payload);

//     if(strcmp(topic, "garmentbot/mode") == 0) {   // char len = 17
//         GarmentBot::GARMENTBOT_STATE mode = (GarmentBot::GARMENTBOT_STATE)(atoi(cc));
//         mybot->ToState(mode);
//     }
//     else if(strcmp(topic, "smokebot/pause_second")==0){    // char len = 21

//     }else{
//         Serial.print(" app_mqtt_received_message() Warning   ");
//         Serial.print (topic);
//         Serial.print("   ");
//         Serial.print (payload);
//     }

// }


void setup(){
    Serial.begin(115200);
    Serial.println("Hi there, I am your lovely bot,  Garmentbot AGV + BoxMover.  Keep smiling :)");
    mybot = new GarmentBot();
    mybot->Init();
    Begin_WifiRabbitMqtt();

    Serial.println ("\n\n  Setup_wifi_mqtt is done. ------------------------------------ ");

}


void loop(){
    mybot->SpinOnce();


}

#endif