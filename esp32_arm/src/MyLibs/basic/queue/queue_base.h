#pragma once
#include "MyLibs/basic/logger.h"
        
// class Queue_able{
//     public:
//         int id = 1234;
//         // virtual void DeepReset_ToDefault();   // Why this cause exception?

// };

//TODO:  make a template, to remove Queue_able.
template <class T>
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
        T* FetchTailMessage(bool takeout_from_queue){};

        //Push the appended object to queue. 
        bool Deposit();   //Push()?   Enqueue()?   Deposit()?
        void PrintOut(const char * title);
        
    protected:
        // When you GetPreHeadObject(), you can view/check/modify it, also can ForwardHead(), equal to AppendObject().
        T* _GetRoom();  //GetPrePush()?  GetRoom()?  
        T* _Withdraw();  //Pop()?  Withdraw()?  Dequeue?
        T* _GetHeadObject();  //Pop()?  Withdraw()?  Dequeue?

        void _Init(const char* queue_name, int queue_size, T* first_element);
        T* __all_elements;

    private:
        int __get_next_index(int current_index);
        int __get_previous_index(int current_index);
        int __queue_size = 0;
        int __element_size;   //todo: element_size
        int __push_head = 0;
        int __pop_tail = 0;
        const char* __queue_name;

};


template <class T>
void QueueBase<T>::_Init(const char* queue_name, int queue_size, T* first_element){
    this->__queue_name = queue_name;
    this->__queue_size = queue_size; 
    this->__element_size = sizeof(T);
    this->__all_elements = first_element;
    this->PrintOut("QueueBase:_Init()");
}

template <class T>
void QueueBase<T>::PrintOut(const char * title){
    Logger::Info(title);
    Logger::Print("queue_name", this->__queue_name);
    Logger::Print("items_count",this->__queue_size);
    Logger::Print("sizeof_item",this->__element_size);
    Logger::Print("__pop_tail", this->__pop_tail);
    Logger::Print("__push_head", this->__push_head);
    // __all_elements[0].PrintOut("12345");   // This is a different between Template and Generics
}

template <class T>
bool QueueBase<T>::Deposit(){
    Logger::Debug("ommonQueue::Deposit()");
    Logger::Print("__queue_name", this->__queue_name);
    Logger::Print("old  __push_head ", this->__push_head);
    int next_head = this->__get_next_index(this->__push_head);
    
    if(next_head == this->__pop_tail){
        Logger::Warn("QueueBase::Deposit() ");
        Serial.print("\n   Buffer is full");
        return true;
    }

    // new_object->DeepCopyTo((T*) (this->__all_elements + this->_push_head * this->_sizeof_item));

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

template <class T>
T* QueueBase<T>::_GetRoom(){
    // Logger::Debug("QueueBase::_GetRoom()");
    T* head_message = this->__all_elements + __push_head * this->__element_size;
    // Serial.println(head_message->id);
    // Logger::Print("QueueBase::_GetRoom() point", 99);
    return  head_message;
}

template <class T>
T* QueueBase<T>::_Withdraw(){
    T* tail_message = NULL;
    if (this->__push_head != this->__pop_tail){
        tail_message = this->__all_elements + this->__pop_tail * this->__element_size;
        this->__pop_tail = this->__get_next_index(this->__pop_tail);
    }
    return tail_message;
}

// Notice:  If buffer is empty, this will return unexpected result.
template <class T>
T* QueueBase<T>::_GetHeadObject(){
    int previous_head =  __get_previous_index(__push_head);
    return  this->__all_elements + previous_head* this->__element_size;
}

template <class T>
bool QueueBase<T>::BufferIsEmpty(){
    if (this->__push_head == this->__pop_tail) 
        return true;
    return false;
}

template <class T>
bool QueueBase<T>::BufferIsFull(){
    int next_head = this->__get_next_index(this->__push_head);
    if (next_head == this->__pop_tail)
        return true;
    return false;
}

template <class T>
int QueueBase<T>::GetFreeBuffersCount(){
    Logger::Debug("QueueBase<T>::GetFreeBuffersCount()");
    int count = this->__push_head - this->__pop_tail;
    if (count >= 0){
        return this->__queue_size - count;
    }
    
    return 0;
}

template <class T>
int QueueBase<T>::__get_next_index(int current_index){
    int next_index = current_index;
    next_index++;
    if (next_index == this->__queue_size)   
        // out of range.
        next_index = 0;
    return next_index;
}

template <class T>
int QueueBase<T>::__get_previous_index(int current_index){
    int next_index = current_index;
    next_index--;
    if (next_index <0 )   
        // out of range.
        next_index = this->__queue_size - 1;
    return next_index;
}
