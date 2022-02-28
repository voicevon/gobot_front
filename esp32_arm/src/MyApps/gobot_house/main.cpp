#include "all_devices.h"
#ifdef I_AM_GOBOT_HOUSE

// #include "Robot/Commu/CommuBleGattServer.h"
#include "gobot_house.h"
#include "MyLibs/MyFunctions.hpp" 
#include "Robot/command_queue_rabbit.h"
#include "IoT/wifi_mqtt_client.h"
// static char LOG_TAG[]= "BLE-HOUSE";
GobotHouse* mybot; 
RobotAction action;
CommandQueueRabbit* commandQueueRabbit;


extern AsyncMqttClient mqttClient;
void dispatch_MqttConnected(bool sessionPresent){
    commandQueueRabbit->SubscribeMqtt(&mqttClient);
}
// void onMqttConnect(bool sessionPresent) {
void dispatch_MqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) {
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

    commandQueueRabbit->OnReceived(payload,20);
}

void setup(){
    Serial.begin(115200);
    Serial.println("Hi Xuming, I am your lovely bot,  GobotHouse. ");
    mybot = &GobotHouse::getInstance();
    mybot->Setup(&action);
    // mybot->Calibrate(6,true);
    // for (int i=17; i>=10;i--){
    //     mybot->Calibrate(i, false);
    // }
	mybot->ParkArms(true);

    setup_wifi_mqtt();
    commandQueueRabbit = new CommandQueueRabbit();
	commandQueueRabbit->LinkLocalCommandQueue(mybot->GetCommandQueue());
    mqttClient.onConnect(dispatch_MqttConnected);
    mqttClient.onMessage(dispatch_MqttMessage);
    // AsyncMqttClient* pMqttClient = &mqttClient;
    // mqttClient.onMessage(commandQueueRabbit->onMqttMessage);
    // commandQueueRabbit->LinkMqttClinet(pMqttClient);
}

bool done= false;
void loop(){
	// WebCommu_SpinOnce();
	mybot->SpinOnce();
    // done = mybot->MoveStone_FromRoomToHead(0);
    // if(done) Serial.print("Done to 0");
    // done = mybot->MoveStone_FromHeadToRoom(0);
    // if(done) Serial.print("Done to head");
    // mybot->MoveStone_FromRoomToHead(2);
    // mybot->MoveStone_FromRoomToHead(3);
    // mybot->MoveStone_FromRoomToHead(4);
    // mybot->MoveStone_FromRoomToHead(5);
    // mybot->MoveStone_FromRoomToHead(6);
    // mybot->MoveStone_FromRoomToHead(7);
}


#endif