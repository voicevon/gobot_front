#pragma once

#include "CNC/gcode/gcode_queue.h"

// This is a local message producer
// Normally, this producer 
//     1. Take message from rabbitMQ or MQTT
//     2. Act as a message producer, put the message to local message queue.

class GcodeProducer{
    public:
        void LinkLocalGcodeQueue_AsProducer(GcodeQueue* gcode_queue) {
            this->_gcode_queue = gcode_queue;
        };

    protected:
        GcodeQueue* _gcode_queue;

    private:


};
