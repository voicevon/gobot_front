#pragma once
#define MESSAGE_COUNT_IN_QUEUE 200
#include "MyLibs/basic/logger.h"
        
class Queue_able{
    public:
        virtual void DeepCopyTo(Queue_able* the_copy);
};


class CommonQueue{
    public:
        // For Producer:    
        bool AppendObject(Queue_able* new_object);
        bool BufferIsFull();
        int GetFreeBuffersCount();

        // For Consumer:
        bool BufferIsEmpty();  //TODO: remove this function?
        Queue_able* FetchTailObject();

        // For both Producer and Consumer:
        CommonQueue(){};
        Queue_able* GetHeadObject();

    protected:
        virtual Queue_able* GetSubClass();
        // This is a buffer , can contain 20 bytes per command, and 22 commands in the queue.
        Queue_able _all_messages[MESSAGE_COUNT_IN_QUEUE];
        int _head = 0;
        int _tail = 0;

    private:
        int __get_pointer_next_index(int current_index);
        int __get_pointer_previous_index(int current_index);

};