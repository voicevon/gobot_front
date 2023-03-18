#pragma once
// #include "mqtt/wifi_mqtt_client.h"
#include "mqtt_subscriber_base.h"




class MqttSubscriberManager{
    public:
        static MqttSubscriberManager& GetInstance(){
            static MqttSubscriberManager __instance;
            return __instance;
        };
        void AddSubscriber(const char* mqtt_topic, MqttSubscriberBase* subscriber);
        void on_mqtt_client_received_message(char* topic, char* payload,  size_t payload_len);

    private:
        MqttSubscriberBase* __all_subscribers[20];
        int __subscriber_count = 0;

};