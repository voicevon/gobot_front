#pragma once
#define MAX_BYTES_PER_MESSAGE 20
#define MESSAGE_COUNT_IN_QUEUE 200
#include <WString.h>
#include "MyLibs/basic/logger.h"


class VonMessage{
    public:
        String message_target;  // 'app' or 'robot'
        String message_content;    // command or gcode,mcode
};

class VonMessageQueue{
    public:
        // For both Producer and Consumer:
        VonMessageQueue(){};
        VonMessage* GetHeadMessage();
        void SayHello(String title);

        // For Producer:    
        // bool AppendMessage(String payload);
        // bool AppendMessage(const char* payload, int length);
        bool AppendMessage(VonMessage* message);
        bool BufferIsFull();
        int GetFreeBuffersCount();

        // For Consumer:
        bool BufferIsEmpty();  //TODO: remove this function?
        VonMessage* FetchTailMessage(bool takeout_from_queue);


    protected:
        // This is a buffer , can contain 20 bytes per command, and 22 commands in the queue.
        VonMessage _all_messages[MESSAGE_COUNT_IN_QUEUE];
        int _head = 0;
        int _tail = 0;

    private:
        int __get_pointer_next_index(int current_index);
        int __get_pointer_previous_index(int current_index);

};