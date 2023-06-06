#pragma once

#include "MyLibs/basic/queue/queue_base.h"
#include "MyLibs/basic/c_string/c_string_base.h"

#define COMMAND_QUEUE_SIZE 16

class CommandText: public C_String_Base{
    public:
        CommandText(): C_String_Base(50){};
        // const char* GetChars(){return __all_chars;};

    private:
        char __all_chars[50];
};

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