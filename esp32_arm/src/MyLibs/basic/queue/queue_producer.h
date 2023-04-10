#pragma once

#include "queue_base.h"

// This is a local message producer
// Normally, this producer 
//     1. Take message from rabbitMQ or MQTT
//     2. Act as a message producer, put the message to local message queue.

class CommonQueueProducer{
    public:
        void LinkQueue_AsProducer(QueueBase* the_queue) {this->_queue = the_queue;};

    protected:
        QueueBase* _queue;

    private:


};
