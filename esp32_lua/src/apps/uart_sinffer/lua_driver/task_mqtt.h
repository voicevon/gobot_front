#pragma once

#include <AsyncMqttClient.h>
#include "MyLibs/basic/text/mqtt_topic.h"


class Subscriber{
    
}
class SmartMqttClient{
    public:
        static void mqtt_publish(const char* topic, const char* payload);
        static void mqtt_subscribe(const char* topic);
        static int mqtt_read_payload(const int topic_id, char* payload);
        static void mqtt_release_buffer(const int topic_id);
        int Get_Payload_bits(){return 1;};
        void connectToMqtt();
        void Init();

    private:
        static bool mqtt_is_connected;
        static MqttTopic mqtt_topics[20];

        static void onMqttConnect(bool sessionPresent);
        static void onMqttDisconnect(AsyncMqttClientDisconnectReason reason) ;
        static void onMqttSubscribe(uint16_t packetId, uint8_t qos) ;
        static void onMqttUnsubscribe(uint16_t packetId) ;
        static void onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) ;
        static void onMqttPublish(uint16_t packetId) ;


};


extern AsyncMqttClient g_mqttClient;  //TODO:  be a static method of a class

extern void TaskMqtt(void* parameter);
