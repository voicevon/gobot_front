#pragma once
// #include "queue_base.h"
#include "MyLibs/basic/queue/queue_base.h"


class MqttMessage{
    public:
        int id;
        const char * payload;
        int length;
};

class MqttMessageQueue: public QueueBase<MqttMessage>{
    public:
        bool AppendMessage(String payload){};
        bool AppendMessage(const char* payload, int length){};
        MqttMessage* GetHeadMessage(){};

    private:


};