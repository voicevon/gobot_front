#include "all_devices.h"
#ifdef I_AM_GARMENT_BOT


#include "MyLibs/MyFunctions.hpp"
#include "garment_bot.h"



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
CommandQueueRabbit* mq_box_mover;
CommandQueueRabbit* mq_twin_wheels;
extern AsyncMqttClient mqttClient;
bool mqtt_is_connected = false;
void dispatch_MqttConnected(bool sessionPresent){
    Serial.println("\n\n     MQTT is connected !!!!\n\n");
    mqtt_is_connected = true;
    mq_box_mover->SubscribeMqtt(&mqttClient, "agv/x2206", "agv/x2206/fb");
    mq_box_mover->SubscribeMqtt(&mqttClient, "agv/x2206/box", "agv/x2206/box/fb");
    mq_twin_wheels->SubscribeMqtt(&mqttClient, "agv/x2206/agv", "agv/x2206/agv/fb");
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
    if (1==1){
        mq_box_mover->OnReceived(payload, len);
    }else{
        mq_twin_wheels->OnReceived(payload, len);
    }
}
void Begin_WifiRabbitMqtt(){
    setup_wifi_mqtt();
    mq_box_mover = new CommandQueueRabbit();
    mq_box_mover->LinkLocalCommandQueue(mybot->objBoxMover.GetCommandQueue());
    mq_twin_wheels = new CommandQueueRabbit();
    mq_twin_wheels->LinkLocalCommandQueue(mybot->objTwinWheel.GetCommandQueue());
    mqttClient.onConnect(dispatch_MqttConnected);
    mqttClient.onMessage(dispatch_MqttMessage);
}
//*********************  end of MQTT and RabbitMQ  *******************************************


void setup(){
    Serial.begin(115200);
    Serial.println("Hi there, I am your lovely bot,  Garmentbot AGV + BoxMover.  Keep smiling :)");
    mybot = new GarmentBot();
    mybot->Init();
    Begin_WifiRabbitMqtt();
    while (! mqtt_is_connected){
        delay(100);
    }
    Serial.println ("\n\n  Setup_wifi_mqtt is done. ------------------------------------ ");
}


void loop(){
    mybot->SpinOnce();
}

#endif