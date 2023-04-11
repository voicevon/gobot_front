#pragma once

#include "Gcode.h"
#include "MyLibs/basic/queue/message_queue.h"

class GcodeQueue: public MessageQueue{
    public:
        GcodeQueue(){};
        
        // TODO: BLOCKING mode or UNBLOCKING mode
        // return true:   buffer is full
        // return false:  buffer is not full
        bool AppendGcodeCommand(String command){
            return this->AppendMessage(command);
        };

        bool AppendGcodeCommand(const char* payload, int length){
            return this->AppendMessage(payload, length);
        };

    private:
        
};