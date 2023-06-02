#pragma once
#include "stdint.h"
#include "MyLibs/utility/logger.h"
#include "wifi_mqtt_client.h"

class MqttSubscriberBase{
    public:
        void SetMqttTopic(const char* mqtt_topic);
        bool IsTopicEqualTo(const char* mqtt_topic);
        const char* GetMqttTopic(){return _mqtt_topic;};
        virtual void onGot_MqttMessage(const char* payload, uint16_t payload_len);

    //     void LinkLocalMq_AsMqttMessageConsumer(MqttMessageQueue* mq){this->__mq = mq;};
    //     virtual void SpinOnce();

    // protected:
    //     // virtual void onGot_MqttMessage(const char* mqtt_payload);
    //     bool CheckMqttCommand(){};

        
    protected:
        const char* _mqtt_topic;
};

