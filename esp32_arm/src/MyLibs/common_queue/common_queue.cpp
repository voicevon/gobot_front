#include "common_queue.h"
#include "HardwareSerial.h"
#include "MyBoards/const.h"


// return true: buffer is full , before or after appending.
// return false:  buffer is not full, after appending.
bool CommonQueue::AppendObject(Queue_able* new_object){
    int next_head = this->__get_pointer_next_index(this->_head);
    if(next_head == this->_tail){
        Serial.print(FORE_YELLOW);
        Serial.print("\n  [Warn] CommonQueue::AppendMessage() ");
        Serial.print("\n   Buffer is full");
        Serial.println(FCBC_RESET);
        return true;
    }

    new_object->DeepCopyTo((Queue_able*) (this->_all_queue_ables + this->_head * this->_sizeof_item));

    this->_head = next_head;
    next_head = this->__get_pointer_next_index(this->_head);
    if (next_head == this->_tail)
        // buffer is full, after copying.
        return true;
    // buffer is NOT full. 
    return false;  
}

Queue_able* CommonQueue::GetHeadObject(){
    int previous_head = this->__get_pointer_previous_index(this->_head);
    // Queue_able* head_message = this->_all_queue_ables[previous_head];
    Queue_able* head_message =(Queue_able*)(this->_all_queue_ables + previous_head * this->_sizeof_item);

    return  head_message;
}

Queue_able* CommonQueue::FetchTailObject(){
    Queue_able* tail_message = NULL;
    if (this->_head != this->_tail){
        // tail_message = this->_all_queue_ables[this->_tail];
        tail_message = (Queue_able*) (this->_all_queue_ables + this->_tail * this->_sizeof_item);
        this->_tail = this->__get_pointer_next_index(this->_tail);
    }
    return tail_message;
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

