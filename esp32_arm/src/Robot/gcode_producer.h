#pragma once

#include "MyLibs/message_producer.h"
#include "Robot/gcode_queue.h"

class GcodeProducer: public MessageProducer{
    public:
        GcodeQueue* GetCommandQueue(){return this->_gcode_queue;};

    protected:
        GcodeQueue* _gcode_queue;

    private:


};
