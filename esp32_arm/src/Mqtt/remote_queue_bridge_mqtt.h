#pragma once

#include <Arduino.h>
#include "MyLibs/basic/queue/message_queue.h"
#include "Mqtt/wifi_mqtt_client.h"
#include "mqtt_subscriber_base.h"

class RemoteQueueBridge_mqtt: public MqttSubscriberBase{
    public:
        // void Init(AsyncMqttClient* mqttClient, const char* main_topic, const char* feedback_topic);
        void Init(const char* feedback_topic);  // TODO:;  get feedback_topic from subscriber topic inside class.
        void LinkLocalCommandQueue_AsMqttMessageProducer(MessageQueue* loacalMQ);  //, String mqtt_topic);

        // void OnReceived(const char* payload, int length);
        void SpinOnce();
        void onMessage(const char* payload, uint16_t payload_len) override;

        
    private:
        // AsyncMqttClient* __mqttClient;
        String __feedback_topic;   // Be honest, I love and hate Arduino.String()

        MessageQueue* __localMQ;
        bool __local_mq_is_full = false;
        int __newest_payload_id = -1;
};