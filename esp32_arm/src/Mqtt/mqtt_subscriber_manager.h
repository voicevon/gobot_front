#pragma once
#include "mqtt/wifi_mqtt_client.h"
#include "mqtt_subscriber_base.h"

// class MqttSubscriberConfig{
//     public:
//         const char* mqtt_topic; 
//         void* callback_rx_service;
//         bool EqualToTopic(const char* mqtt_topic){return true;};


//     private:
// };


class MqttSubscriberManager{
    public:
        static MqttSubscriberManager& GetInstance(){
            static MqttSubscriberManager __instance;
            return __instance;
        };
        void AddSubscriber(const char* mqtt_topic, MqttSubscriberBase* subscriber);
        void on_mqtt_client_received_message(char* topic, char* payload,  size_t payload_len);

        //TODO:  remove subscriber.  
        // void RemoveSubscriber(const char* mqtt_topic);
        // void RemoveSubscriber(MqttSubscriberConfig* subscriber);

    private:
        MqttSubscriberBase* __all_subscribers[20];
        // MqttSubscriberConfig __all_subscribers[20];

};