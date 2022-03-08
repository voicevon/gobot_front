#pragma once
/*
.                                      |----------  MyLastedMessage <-----|
GobotHead -------> HouseQueue -----> Agent  -------> HouseMQTT -------> HouseBot
            |
            |----> ArmQueue 
.




*/

#include <Arduino.h>
#include "gcode_queue.h"
#include "IoT/wifi_mqtt_client.h"


class CommandQueueRabbit{
    public:
        String topic;
        String topic_feedback;
        byte* payload;
        void LinkLocalCommandQueue(GcodeQueue* loacalMQ);  //, String mqtt_topic);
        void SubscribeMqtt(AsyncMqttClient* mqttClient, const char* main_topic, const char* feedback_topic);

        void OnReceived(char* payload, int length);
        void SpinOnce();
    private:
        GcodeQueue* __localMQ;
        AsyncMqttClient* __mqttClient;
        bool __local_mq_is_full = false;
};