#include "mqtt_subscriber_manager.h"

void MqttSubscriberManager::on_mqtt_client_received_message(char* topic, char* payload,  size_t payload_len){
    MqttSubscriberBase* subscriber;
    // try to find who is subscribing this topic
    for (int i=0; i<6; i++){
        if (subscriber->IsTopicEqualTo(topic)){
            subscriber->onMessage(payload, payload_len);
        }
    }
}

void MqttSubscriberManager::AddSubscriber(const char* mqtt_topic, MqttSubscriberBase* subscriber){
    subscriber->SetMqttTopic(mqtt_topic);
}

