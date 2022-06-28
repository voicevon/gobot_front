#pragma once
#include "gcode_queue.h"
#include "MyLibs/basic/logger.h"
class GcodeConsumer{
    public:
        void LinkLocalGcodeQueue_AsConsumer(GcodeQueue* gcode_queue) {this->_gcode_queue =gcode_queue;};

    // protected:
        GcodeQueue* _gcode_queue;
};