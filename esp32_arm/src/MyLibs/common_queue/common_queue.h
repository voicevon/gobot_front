#pragma once
#include "MyLibs/basic/logger.h"
        
class Queue_able{
    public:
        int id = 1234;
        virtual void DeepReset_ToDefault(){};

};

//TODO:  make a template, to remove Queue_able.
class CommonQueue{
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

    protected:
        // When you GetPreHeadObject(), you can view/check/modify it, also can ForwardHead(), equal to AppendObject().
        Queue_able* _GetRoom();  //GetPrePush()?  GetRoom()?  
        Queue_able* _Withdraw();  //Pop()?  Withdraw()?  Dequeue?
        Queue_able* _GetHeadObject();  //Pop()?  Withdraw()?  Dequeue?

        int _head = 0;
        int _tail = 0;
        void _Init(int items_count, int sizeof_item){
            Logger::Info("CommonQueue:_Init()");
            this->__MESSAGE_COUNT_IN_QUEUE = items_count; 
            this->_sizeof_item = sizeof_item;
            Logger::Print("items_count",items_count);
            Logger::Print("sizeof_item",sizeof_item);
            };

    private:
        int __get_pointer_next_index(int current_index);
        int __get_pointer_previous_index(int current_index);
        int __MESSAGE_COUNT_IN_QUEUE=0;
        int _sizeof_item;

};