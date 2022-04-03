#pragma once
#include "MyLibs/message_queue.h"

class MqttMessageConsumer{
    public:
        void LinkLocalMq_AsMqttMessageConsumer(MessageQueue* mq){this->__mq = mq;};

    private:
        MessageQueue* __mq;
};