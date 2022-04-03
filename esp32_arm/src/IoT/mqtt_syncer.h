#pragma once
/*
.                                      |----------  MyLastedMessage <-----|
GobotHead -------> HouseQueue -----> Agent  -------> HouseMQTT -------> HouseBot
            |
            |----> ArmQueue 
.




*/

#include <Arduino.h>
// #include "gcode_queue.h"
#include "MyLibs/message_queue.h"
#include "IoT/wifi_mqtt_client.h"


class MqttSyncer{
    public:
        String topic;
        String topic_feedback;
        byte* payload;
        void LinkLocalCommandQueue_AsMqttMessageProducer(MessageQueue* loacalMQ);  //, String mqtt_topic);
        void SubscribeMqtt(AsyncMqttClient* mqttClient, const char* main_topic, const char* feedback_topic);

        void OnReceived(const char* payload, int length);
        void SpinOnce();
    private:
        MessageQueue* __localMQ;
        AsyncMqttClient* __mqttClient;
        bool __local_mq_is_full = false;
};