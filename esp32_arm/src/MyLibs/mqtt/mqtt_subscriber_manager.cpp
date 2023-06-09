#include "mqtt_subscriber_manager.h"

void gs_MqttSubscriberManager::on_mqtt_client_received_message(char* topic, char* payload,  size_t payload_len){
    MqttSubscriberBase* subscriber;
    // Logger::Debug("gs_MqttSubscriberManager::on_mqtt_client_received_message()");
    for (int i=0; i<__subscriber_count; i++){
        // try to find who is subscribing this topic
        subscriber = __all_subscribers[i];
        if (subscriber->IsTopicEqualTo(topic)){
            // Logger::Print("Got Mqtt Subscriber of topic:", subscriber->GetMqttTopic());
            subscriber->onGot_MqttMessage(payload, payload_len);
            return;
        }
    }
    Logger::Warn("gs_MqttSubscriberManager::on_mqtt_client_received_message().  Out of my managerment");
    Logger::Print(topic, payload);

}

void gs_MqttSubscriberManager::AddSubscriber(const char* mqtt_topic, MqttSubscriberBase* subscriber){
    __all_subscribers[__subscriber_count] = subscriber;
    subscriber->SetMqttTopic(mqtt_topic);
    g_mqttClient.subscribe(mqtt_topic, 2);

    __subscriber_count++;
    if (__subscriber_count >=20){
        Logger::Warn(" gs_MqttSubscriberManager::AddSubscriber   will overflow if put another subscriber");
    }
}

