#include "mqtt_subscriber_manager.h"

void MqttSubscriberManager::on_mqtt_client_received_message(char* topic, char* payload,  size_t payload_len){
    MqttSubscriberBase* subscriber;
    Logger::Debug("MqttSubscriberManager::on_mqtt_client_received_message");
    for (int i=0; i<__subscriber_count; i++){
        // try to find who is subscribing this topic
        subscriber = __all_subscribers[i];
        if (subscriber->IsTopicEqualTo(topic)){
            Logger::Print("Got Mqtt Subscriber", subscriber->GetMqttTopic());
            subscriber->onMessage(payload, payload_len);
        }
    }
}

void MqttSubscriberManager::AddSubscriber(const char* mqtt_topic, MqttSubscriberBase* subscriber){
    __all_subscribers[__subscriber_count] = subscriber;
    subscriber->SetMqttTopic(mqtt_topic);
    __subscriber_count++;
    if (__subscriber_count >=20){
        Logger::Warn(" MqttSubscriberManager::AddSubscriber   will overflow if put another subscriber");
    }
}

