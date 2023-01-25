#pragma once
#include "MyLibs/message_queue.h"

class MqttMessageConsumer{
    public:
        void LinkLocalMq_AsMqttMessageConsumer(MessageQueue* mq){this->__mq = mq;};
        virtual void SpinOnce();

    protected:
        virtual void onGot_MqttMessage(const char* mqtt_payload);
        bool CheckMqttCommand();
        
    private:
        MessageQueue* __mq;
};