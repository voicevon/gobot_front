#include "mqtt_subscriber_manager.h"
#include "mqtt_client.h"
#include "von/cpp/utility/logger.h"
#include "von/cpp/mqtt/task_mqtt.h"


MqttSubscriberBase* gs_MqttSubscriberManager::__find_subscriber(const char* topic){

    MqttSubscriberBase* subscriber;
    // Logger::Debug("gs_MqttSubscriberManager::on_mqtt_client_received_message()");
    for (int i=0; i<__subscriber_count; i++){
        // try to find who is subscribing this topic
        subscriber = __all_subscribers[i];
        // if (subscriber->IsTopicEqualTo(topic)){
        if (subscriber->GetMqttTopic()->IsEqualTo(topic)){
            return subscriber;
        }
    }
    Logger::Error("gs_MqttSubscriberManager::on_mqtt_client_received_message().  Out of my managerment");
    Logger::Print("mqtt topic", topic);
    return nullptr;
}

void gs_MqttSubscriberManager::on_mqtt_client_received_message(char* topic, char* payload,  size_t payload_len){
    MqttSubscriberBase* subscriber = __find_subscriber(topic);
    __mqttPayloadBuffer.clear();
    for (int i=0; i<payload_len; i++){
        __mqttPayloadBuffer.push_back(*(payload+i));
    }
    __mqttPayloadBuffer.push_back(char(0x00));
    subscriber->onGot_MqttMessage(&__mqttPayloadBuffer[0], __mqttPayloadBuffer.size());
}

void gs_MqttSubscriberManager::on_mqtt_client_received_message(char* topic, char* payload,  size_t len, size_t index, size_t total){
    MqttSubscriberBase* subscriber = __find_subscriber(topic);
    if (subscriber->GetMqttTopic()){
        //TODO:  confirmed is long payload more than 1kb
        //For this project, only one big payload can be transfered at a time.
        //   Saying, the first payload must be finished, before starting the second message
        // solution: put the buffer into subsriber. not the manager
    }
	if (index == 0) {
        __mqttPayloadBuffer.clear();
    }
    for (int i=0; i<len; i++){
        __mqttPayloadBuffer.push_back(*(payload+i));
    }

	if (index + len != total) return;  // return if payload buffer is not complete
    
	// here, our mqttPayloadBuffer is compplete
    __mqttPayloadBuffer.push_back(char(0x00));
    Logger::Info("check whole content");
    Logger::Print("", &__mqttPayloadBuffer[0]);
    subscriber->onGot_MqttMessage(&__mqttPayloadBuffer[0], __mqttPayloadBuffer.size());

}


void gs_MqttSubscriberManager::AddSubscriber(const char* mqtt_topic, MqttSubscriberBase* subscriber){
    __all_subscribers[__subscriber_count] = subscriber;
    subscriber->SubscribeMqtt(mqtt_topic);
    g_mqttClient.subscribe(mqtt_topic, 2);

    __subscriber_count++;
    if (__subscriber_count >=20){
        Logger::Warn(" gs_MqttSubscriberManager::AddSubscriber   will overflow if put another subscriber");
    }
}

