#pragma once
#include "stdint.h"
#include "MyLibs/basic/logger.h"
#include "wifi_mqtt_client.h"

class MqttSubscriberBase{
    public:
        virtual void onMessage(const char* payload, uint16_t payload_len);
        void SetMqttTopic(const char* mqtt_topic);
        bool IsTopicEqualTo(const char* mqtt_topic);

    protected:
        const char* _mqtt_topic;
};

