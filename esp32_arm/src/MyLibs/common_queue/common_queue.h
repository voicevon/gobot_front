#pragma once
// #define MESSAGE_COUNT_IN_QUEUE 200
#include "MyLibs/basic/logger.h"
        
class Queue_able{
    public:
        virtual void DeepCopyTo(Queue_able* the_copy){};  //TODO: Can remove body "{}"?
};

//TODO:  make a template, to remove Queue_able.
class CommonQueue{
    public:
        // For Producer: 

        // For Producer:  Will deep copy the object to the queue.
        // return true: buffer is full , before or after appending.
        // return false:  buffer is not full, after appending.   
        bool AppendObject(Queue_able* new_object);
        bool BufferIsFull();
        int GetFreeBuffersCount();
        // When you GetHeadObject(), you can view/check it, also can ForwardHead(), equal to AppendObject().
        bool ForwardHead();

        // For Consumer:
        bool BufferIsEmpty();  //TODO: remove this function?
        Queue_able* FetchTailObject();

        // For both Producer and Consumer:
        Queue_able* GetHeadObject();

        
        Queue_able* _all_queue_ables;

    protected:
        // virtual Queue_able* GetSubClass();
        int _head = 0;
        int _tail = 0;
        int _sizeof_item;
        void _Init(int items_count, int sizeof_item){this->__MESSAGE_COUNT_IN_QUEUE = items_count; this->_sizeof_item=sizeof_item;};

    private:
        int __get_pointer_next_index(int current_index);
        int __get_pointer_previous_index(int current_index);
        int __MESSAGE_COUNT_IN_QUEUE=0;

};