#pragma once

#include "MyLibs/basic/queue/queue_base.h"
#include "gcode_text.h"

#define GCODE_QUEUE_SIZE 16

class GcodeQueue: public QueueBase<GcodeText>{
    public:
        GcodeQueue();
        int AppendGcodeCommand(String command);

        // returns:
        //    -1:  queue is full
        //    -2:  command is over-size.
        //     1:  deposited to queue successful.
        int AppendGcodeCommand(const char* command);
        int AppendGcodeCommand(const char* payload, int length);
        void PrintOut_GcddeText(const char* title);
    private:
        GcodeText __all_gcodes[GCODE_QUEUE_SIZE];
        
};