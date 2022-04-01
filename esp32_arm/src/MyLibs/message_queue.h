#pragma once
#define MAX_BYTES_PER_MESSAGE 20
#define MESSAGE_COUNT_IN_QUEUE 22
// #include <Arduino.h>  //for String only
#include <WString.h>


class MessageQueue{
    public:
        struct SingleMessage{
            int id;
            char payload[MAX_BYTES_PER_MESSAGE];
            int length;
        };
        // For Producer:    
        bool AppendMessage(String payload);
        bool AppendMessage(const char* payload, int length);
        bool BufferIsFull();
        int GetFreeBuffersCount();

        // For Consumer:
        bool BufferIsEmpty();  //TODO: remove this function?
        SingleMessage* FetchTailMessage();

        // For both Producer and Consumer:
        SingleMessage* GetHeadMessage();
        void SayHello(String title);

    protected:
        // This is a buffer , can contain 20 bytes per command, and 22 commands in the queue.
        SingleMessage _all_messages[MESSAGE_COUNT_IN_QUEUE];
        int _head = 0;
        int _tail = 0;

    private:
        int __get_pointer_next_index(int current_index);
        int __get_pointer_previous_index(int current_index);

};