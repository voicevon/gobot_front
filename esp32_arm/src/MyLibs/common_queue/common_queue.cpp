#include "common_queue.h"
#include "MyLibs/basic/logger.h"


bool CommonQueue::Deposit(){
    // Logger::Debug("ommonQueue::Deposit()");
    // Logger::Print("head old", this->_head);
    // Logger::Print("tail", this->_tail);
    int next_head = this->__get_pointer_next_index(this->_head);
    if(next_head == this->_tail){
        Logger::Warn("CommonQueue::Deposit() ");
        Serial.print("\n   Buffer is full");
        return true;
    }

    // new_object->DeepCopyTo((Queue_able*) (this->_all_queue_ables + this->_head * this->_sizeof_item));

    this->_head = next_head;
    // Logger::Print("head new", this->_head);
    next_head = this->__get_pointer_next_index(this->_head);
    if (next_head == this->_tail)
        // buffer is full, after copying.
        return true;
    // buffer is NOT full. 
    return false;  
}


Queue_able* CommonQueue::_GetRoom(){
    // Logger::Debug("CommonQueue::_GetRoom()");
    Queue_able* head_message =(Queue_able*)(this->_all_queue_ables + _head * this->_sizeof_item);
    // Serial.println(head_message->id);
    // Logger::Print("CommonQueue::_GetRoom() point", 99);
    return  head_message;
}

Queue_able* CommonQueue::_Withdraw(){
    Queue_able* tail_message = NULL;
    if (this->_head != this->_tail){
        tail_message = (Queue_able*) (this->_all_queue_ables + this->_tail * this->_sizeof_item);
        this->_tail = this->__get_pointer_next_index(this->_tail);
    }
    return tail_message;
}

// Notice:  If buffer is empty, this will return unexpected result.
Queue_able* CommonQueue::_GetHeadObject(){
    int previous_head =  __get_pointer_previous_index(_head);
    return (Queue_able*) (this->_all_queue_ables + previous_head* this->_sizeof_item);
}

bool CommonQueue::BufferIsEmpty(){
    if (this->_head == this->_tail) 
        return true;
    return false;
}

bool CommonQueue::BufferIsFull(){
    int next_head = this->__get_pointer_next_index(this->_head);
    if (next_head == this->_tail)
        return true;
    return false;
}

int CommonQueue::GetFreeBuffersCount(){
    int count = this->_head - this->_tail;
    if (count >= 0)
        return this->__MESSAGE_COUNT_IN_QUEUE - count;
    else
        return 0-count;
}

int CommonQueue::__get_pointer_next_index(int current_index){
    int next_index = current_index;
    next_index++;
    if (next_index == this->__MESSAGE_COUNT_IN_QUEUE)   
        // out of range.
        next_index = 0;
    return next_index;
}

int CommonQueue::__get_pointer_previous_index(int current_index){
    int next_index = current_index;
    next_index--;
    if (next_index <0 )   
        // out of range.
        next_index = this->__MESSAGE_COUNT_IN_QUEUE - 1;
    return next_index;
}

