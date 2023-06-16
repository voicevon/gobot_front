#pragma once
// #include "mqtt_subscriber_base.h"
#include "von/cpp/mqtt/mqtt_subscriber_base.h"
#include "stddef.h"  // size_t
#include "vector"


class gs_MqttSubscriberManager{
    public:
        static gs_MqttSubscriberManager& Instance(){
            static gs_MqttSubscriberManager __instance;
            return __instance;
        };
        void AddSubscriber(const char* mqtt_topic, MqttSubscriberBase* subscriber);
        void on_mqtt_client_received_message(char* topic, char* payload,  size_t payload_len);
        void on_mqtt_client_received_message(char* topic, char* payload,  size_t len, size_t index, size_t total);

    private:
        MqttSubscriberBase* __all_subscribers[20];
        int __subscriber_count = 0;
        MqttSubscriberBase* __find_subscriber(const char* topic);
		std::vector<char> __mqttPayloadBuffer = {};


};