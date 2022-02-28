/*
.                                      |----------  MyLastedMessage <-----|
GobotHead -------> HouseQueue -----> Agent  -------> HouseMQTT -------> HouseBot
            |
            |----> ArmQueue 
.




*/

#include <Arduino.h>
#include "command_queue.h"
#include "IoT/wifi_mqtt_client.h"


class CommandQueueRabbit{
    public:
        String topic;
        String topic_feedback;
        byte* payload;
        void LinkLocalCommandQueue(CommandQueue* loacalMQ);  //, String mqtt_topic);
        // void LinkMqttClinet(AsyncMqttClient* mqttClient);
        void SubscribeMqtt(AsyncMqttClient* mqttClient);
        void OnReceived(char* payload, int length);
        // void onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total);


    private:
        CommandQueue* __localMQ;
        AsyncMqttClient* __mqttClient;
};