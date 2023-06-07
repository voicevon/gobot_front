#pragma once

#include "MyLibs/basic/queue/queue_base.h"
#include "text_message_line.h"
#define COMMAND_QUEUE_SIZE 16



class CommandQueue: public QueueBase<CommandText>{
    public:
        CommandQueue();
        int AppendCommand(String command);

        // returns:
        //    -1:  queue is full
        //    -2:  command is over-size.
        //     1:  deposited to queue successful.
        int AppendCommand(const char* command);
        int AppendCommand(const char* payload, int length);
        int AppendCommand(CommandText* gcode_text);
        void PrintOut_CommandText(const char* title);
    private:
        CommandText __all_commands[COMMAND_QUEUE_SIZE];
        
};