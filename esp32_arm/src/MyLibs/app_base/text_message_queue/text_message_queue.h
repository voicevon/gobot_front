#pragma once

#include "MyLibs/basic/queue/queue_base.h"
#include "text_message_line.h"
#define TEXT_MESSAGE_QUEUE_SIZE 16



class TextMessageQueue: public QueueBase<TextMessageLine>{
    public:
        TextMessageQueue();
        int AppendTextMessageLine(String command);

        // returns:
        //    -1:  queue is full
        //    -2:  command is over-size.
        //     1:  deposited to queue successful.
        int AppendTextMessageLine(const char* command);
        int AppendTextMessageLine(const char* payload, int length);
        int AppendTextMessageLine(TextMessageLine* gcode_text);
        void PrintOut_TextMessageLine(const char* title);
        
    private:
        TextMessageLine __all_commands[TEXT_MESSAGE_QUEUE_SIZE];
        
};