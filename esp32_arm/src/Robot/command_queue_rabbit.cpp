#include "command_queue_rabbit.h"

void CommandQueueRabbit::LinkLocalCommandQueue(CommandQueue* loacalMQ){
    this->__localMQ = loacalMQ;
}

void CommandQueueRabbit::SubscribeMqtt(AsyncMqttClient* mqttClient){
    this->__mqttClient = mqttClient;
    mqttClient->subscribe("gobot/x2134/house", 2);
    this->topic_feedback = "gobot/x2134/house/fb";
}

// void CommandQueueRabbit::LinkMqttClinet(AsyncMqttClient* mqttClient){
//     this->__mqttClient =  mqttClient;
//     // setup_wifi_mqtt();
//     // mqttClient->onMessage(this->OnReceived)
//     // mqttClient->onMessage(this->onMqttMessage);
// }

// void CommandQueueRabbit::onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total){
//     this->OnReceived(payload);
// }

// AsyncMqttClient& onMessage(AsyncMqttClientInternals::OnMessageUserCallback callback){

// }


void CommandQueueRabbit::OnReceived(char* payload, int length){
    Serial.println(*payload);
    // Put message to local MQ
    // this->__localMQ->AppendGcodeCommand(payload);
    // send message to feedback topic
    this->__mqttClient->publish(this->topic_feedback.c_str(), 2, true, payload, length);
}