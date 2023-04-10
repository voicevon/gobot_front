#include "queue_base.h"
#include "MyLibs/basic/logger.h"


void QueueBase::_Init(const char* queue_name, int items_count, int sizeof_item){
    this->__MESSAGE_COUNT_IN_QUEUE = items_count; 
    this->__sizeof_item = sizeof_item;
    this->__queue_name = queue_name;
    this->PrintOut("QueueBase:_Init()");
}


void QueueBase::PrintOut(const char * title){
    Logger::Info(title);
    Logger::Print("queue_name", this->__queue_name);
    Logger::Print("items_count",this->__MESSAGE_COUNT_IN_QUEUE);
    Logger::Print("sizeof_item",this->__sizeof_item);
    Logger::Print("__pop_tail", this->__pop_tail);
    Logger::Print("__push_head", this->__push_head);
}

bool QueueBase::Deposit(){
    Logger::Debug("ommonQueue::Deposit()");
    Logger::Print("__queue_name", this->__queue_name);
    Logger::Print("old  __push_head ", this->__push_head);
    int next_head = this->__get_next_index(this->__push_head);
    
    if(next_head == this->__pop_tail){
        Logger::Warn("QueueBase::Deposit() ");
        Serial.print("\n   Buffer is full");
        return true;
    }

    // new_object->DeepCopyTo((Queue_able*) (this->_all_queue_ables + this->_push_head * this->_sizeof_item));

    this->__push_head = next_head;
    next_head = this->__get_next_index(this->__push_head);
    Logger::Print("new  _push_head", this->__push_head);
    this->PrintOut("caller is :  QueueBase::Deposit()");
    if (next_head == this->__pop_tail)
        // buffer is full, after copying.
        return true;
    // buffer is NOT full. 
    return false;  
}


Queue_able* QueueBase::_GetRoom(){
    // Logger::Debug("QueueBase::_GetRoom()");
    Queue_able* head_message =(Queue_able*)(this->_all_queue_ables + __push_head * this->__sizeof_item);
    // Serial.println(head_message->id);
    // Logger::Print("QueueBase::_GetRoom() point", 99);
    return  head_message;
}

Queue_able* QueueBase::_Withdraw(){
    Queue_able* tail_message = NULL;
    if (this->__push_head != this->__pop_tail){
        tail_message = (Queue_able*) (this->_all_queue_ables + this->__pop_tail * this->__sizeof_item);
        this->__pop_tail = this->__get_next_index(this->__pop_tail);
    }
    return tail_message;
}

// Notice:  If buffer is empty, this will return unexpected result.
Queue_able* QueueBase::_GetHeadObject(){
    int previous_head =  __get_previous_index(__push_head);
    return (Queue_able*) (this->_all_queue_ables + previous_head* this->__sizeof_item);
}

bool QueueBase::BufferIsEmpty(){
    if (this->__push_head == this->__pop_tail) 
        return true;
    return false;
}

bool QueueBase::BufferIsFull(){
    int next_head = this->__get_next_index(this->__push_head);
    if (next_head == this->__pop_tail)
        return true;
    return false;
}

int QueueBase::GetFreeBuffersCount(){
    int count = this->__push_head - this->__pop_tail;
    if (count >= 0)
        return this->__MESSAGE_COUNT_IN_QUEUE - count;
    else
        return 0-count;
}

int QueueBase::__get_next_index(int current_index){
    int next_index = current_index;
    next_index++;
    if (next_index == this->__MESSAGE_COUNT_IN_QUEUE)   
        // out of range.
        next_index = 0;
    return next_index;
}

int QueueBase::__get_previous_index(int current_index){
    int next_index = current_index;
    next_index--;
    if (next_index <0 )   
        // out of range.
        next_index = this->__MESSAGE_COUNT_IN_QUEUE - 1;
    return next_index;
}

