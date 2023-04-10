#pragma once
#include "MyLibs/basic/logger.h"
        
class Queue_able{
    public:
        int id = 1234;
        // virtual void DeepReset_ToDefault();   // Why this cause exception?

};

//TODO:  make a template, to remove Queue_able.
// template <class T>
class QueueBase{
    public:
        // For Producer: 

        // For Producer:  Will deep copy the object to the queue.
        // return true: buffer is full , before or after appending.
        // return false:  buffer is not full, after appending.   
        bool BufferIsFull();
        int GetFreeBuffersCount();

        // For Consumer:
        bool BufferIsEmpty();  //TODO: remove this function?

        // For both Producer and Consumer:
        Queue_able* _all_queue_ables;

        //Push the appended object to queue. 
        bool Deposit();   //Push()?   Enqueue()?   Deposit()?
        void PrintOut(const char * title);

    protected:
        // When you GetPreHeadObject(), you can view/check/modify it, also can ForwardHead(), equal to AppendObject().
        Queue_able* _GetRoom();  //GetPrePush()?  GetRoom()?  
        Queue_able* _Withdraw();  //Pop()?  Withdraw()?  Dequeue?
        Queue_able* _GetHeadObject();  //Pop()?  Withdraw()?  Dequeue?

        void _Init(const char* queue_name, int items_count, int sizeof_item);

    private:
        int __get_next_index(int current_index);
        int __get_previous_index(int current_index);
        int __MESSAGE_COUNT_IN_QUEUE = 0;
        int __sizeof_item;
        int __push_head = 0;
        int __pop_tail = 0;
        const char* __queue_name;

};