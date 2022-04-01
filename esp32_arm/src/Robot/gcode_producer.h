#pragma once

// #include "MyLibs/message_producer.h"
#include "Robot/gcode_queue.h"

// This is a local message producer
// Normally, this producer 
//     1. Take message from rabbitMQ or MQTT
//     2. Act as a message producer, put the message to local message queue.

class GcodeProducer{
    public:
        // GcodeQueue* GetGcodeQueue(){return this->_gcode_queue;};
        void LinkLocalMessageQueue(MessageQueue* mq) {this->_gcode_queue = (GcodeQueue*) mq;};

    protected:
        GcodeQueue* _gcode_queue;

    private:


};
