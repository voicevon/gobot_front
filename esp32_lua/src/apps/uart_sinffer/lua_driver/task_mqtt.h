#pragma once

#include <AsyncMqttClient.h>
#include "von/cpp/mqtt/mqtt_topic.h"

class SmartMqttClient{
    public:
        enum EnumState{
            // IDLE = 0,
            CONNECTING,
            CONNECTED,
            DISCONNECTED,
        };
        static SmartMqttClient& Instance(){
            static SmartMqttClient __instance;
            return __instance;
        }
        static void mqtt_publish(const char* topic, const char* payload);
        static void mqtt_subscribe(const char* topic);
        static int mqtt_read_payload(const int topic_id, char* payload);
        static void mqtt_release_buffer(const int topic_id);
        EnumState GetState();
        int Get_Payload_bits(){return 1;};
        void ConnectToBroker();
        void DisconnectFromBroker();
        void Init();
        uint32_t passed_ms(){return millis() - __started_at;};

    private:
        SmartMqttClient(){};
        static bool mqtt_is_connected;
        static MqttTopic mqtt_topics[20];

        uint32_t __started_at = 0;

        static void onMqttConnected(bool sessionPresent);
        static void onMqttDisconnect(AsyncMqttClientDisconnectReason reason) ;
        static void onMqttSubscribe(uint16_t packetId, uint8_t qos) ;
        static void onMqttUnsubscribe(uint16_t packetId) ;
        static void onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) ;
        static void onMqttPublish(uint16_t packetId) ;
};


extern AsyncMqttClient g_mqttClient;  //TODO:  be a static method of a class

extern void TaskMqtt(void* parameter);
