#include "all_devices.h"
#ifdef I_AM_GOBOT_HOUSE

// #include "Robot/Commu/CommuBleGattServer.h"

#include "gobot_house.h"
#include "gobot_house_hw.h"
#include "MyLibs/MyFunctions.hpp" 
GobotHouse* mybot; 
GobotHouseHardware* mybot_hardware;
GcodeQueue* gcode_queue;
// RobotAction action;

// static char LOG_TAG[]= "BLE-HOUSE";


//********************************************************************************************
//    MQTT and RabbitMQ
//********************************************************************************************
#include "Robot/command_queue_rabbit.h"
CommandQueueRabbit* commandQueueRabbit;
extern AsyncMqttClient mqttClient;
bool mqtt_is_connected = false;
void dispatch_MqttConnected(bool sessionPresent){
    Serial.println("\n\n     MQTT is connected !!!!\n\n");
    mqtt_is_connected = true;
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
    commandQueueRabbit->LinkLocalCommandQueue(gcode_queue);
    // commandQueueRabbit->LinkLocalCommandQueue(mybot->GetCommandQueue());
    mqttClient.onConnect(dispatch_MqttConnected);
    mqttClient.onMessage(dispatch_MqttMessage);
}
//*********************  end of MQTT and RabbitMQ  *******************************************




void setup(){
    Serial.begin(115200);
    Serial.println("Hi Xuming, I am your lovely bot,  GobotHouse. ");
    mybot = &GobotHouse::getInstance();
    mybot_hardware = &GobotHouseHardware::getInstance();
    gcode_queue = new GcodeQueue();
    // mybot->Setup(&action);
    mybot->Setup();
    mybot->LinkLocalMessageQueue(gcode_queue);
    mybot_hardware->LinkLocalMessageQueue(gcode_queue);
    Begin_WifiRabbitMqtt();
    while (! mqtt_is_connected){
        delay(100);
    }
}

bool done= false;
void loop(){
	mybot->SpinOnce();
    mybot_hardware->SpinOnce();
    commandQueueRabbit->SpinOnce();
}


#endif