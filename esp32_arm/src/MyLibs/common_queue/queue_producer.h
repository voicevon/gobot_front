#pragma once

#include "common_queue.h"

// This is a local message producer
// Normally, this producer 
//     1. Take message from rabbitMQ or MQTT
//     2. Act as a message producer, put the message to local message queue.

class CommonQueueProducer{
    public:
        void LinkQueue_AsProducer(CommonQueue* the_queue) {this->_queue = the_queue;};

    protected:
        CommonQueue* _queue;

    private:


};
