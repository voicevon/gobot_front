#pragma once

#include <Arduino.h>
#include "MyLibs/message_queue.h"
#include "IoT/wifi_mqtt_client.h"


class RemoteQueueBridge_mqtt{
    public:
        void Init(AsyncMqttClient* mqttClient, const char* main_topic, const char* feedback_topic);
        void LinkLocalCommandQueue_AsMqttMessageProducer(MessageQueue* loacalMQ);  //, String mqtt_topic);

        void OnReceived(const char* payload, int length);
        void SpinOnce();
        
    private:
        AsyncMqttClient* __mqttClient;
        String __feedback_topic;   // Be honest, I love and hate Arduino.String()

        MessageQueue* __localMQ;
        bool __local_mq_is_full = false;
        int __newest_payload_id = -1;
        // String __topic;
        // byte* __payload;
};