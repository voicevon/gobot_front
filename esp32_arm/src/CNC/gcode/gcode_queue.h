#pragma once

#include "Gcode.h"
#include "MyLibs/message_queue.h"

class GcodeQueue: public MessageQueue{
    public:
        GcodeQueue();
        bool AppendGcodeCommand(String command);
        bool AppendGcodeCommand(const char* payload, int length);

    private:
        
};