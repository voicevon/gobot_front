#include "all_devices.h"
#ifdef I_AM_GOBOT_CHESSBOARD

#include "gobot_chessboard.h"
#include "gobot_chessboard_hw.h"
#include "MyLibs/MyFunctions.hpp" 
// #include "Robot/Commu/CommuBleGattServer.h"


// static char LOG_TAG[]= "BLE-HOUSE";
GobotChessboard* mybot; 
GobotChessboardHardware* mybot_hardware;
GcodeQueue* gcode_queue;
// RobotAction action;

//********************************************************************************************
//    MQTT and RabbitMQ
//********************************************************************************************
#include "Robot/mqtt_syncer.h"
MqttSyncer* mqtt_syncer;
extern AsyncMqttClient mqttClient;
void dispatch_MqttConnected(bool sessionPresent){
    mqtt_syncer->SubscribeMqtt(&mqttClient, "gobot/x2134/arm", "gobot/x2134/arm/fb");
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
    mqtt_syncer->OnReceived(payload, len);
}
void Begin_WifiRabbitMqtt(){
    setup_wifi_mqtt();
    mqtt_syncer = new MqttSyncer();
    mqtt_syncer->LinkLocalCommandQueue_AsProducer(gcode_queue);
    mqttClient.onConnect(dispatch_MqttConnected);
    mqttClient.onMessage(dispatch_MqttMessage);
}
//*********************  end of MQTT and RabbitMQ  *******************************************

void setup(){
    Serial.begin(115200);
    Serial.println("Hi Xuming, I am Gobot-Chessboard. Good luck......");
    gcode_queue = new GcodeQueue();
    // setup_hardware();
    // ble.Init();
    // Serial.println("BLE is ok....");    
    mybot = &GobotChessboard::getInstance();
    // mybot->Init(gcode_queue);
    mybot->LinkLocalGcodeQueue_AsProducer(gcode_queue);
    mybot_hardware = new GobotChessboardHardware();
    mybot_hardware->InitRobot();
    mybot_hardware->LinkLocalGcodeQueue_AsConsumer(gcode_queue);
    // mybot->Calibrate(1);
    // mybot->ParkArms(true);
    Begin_WifiRabbitMqtt();
    Serial.print("\nGobot-Chessboard setup is done..........");
}

void loop(){
    mybot->SpinOnce();
    mybot_hardware->SpinOnce();
    mqtt_syncer->SpinOnce();
}
#endif