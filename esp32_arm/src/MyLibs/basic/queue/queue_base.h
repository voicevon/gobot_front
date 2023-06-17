#pragma once
#include "von/utility/logger.h"
        
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
        void PrintOut(const char * title);


        //Push the appended object to queue. 
        bool Deposit();   //Push()?   Enqueue()?   Deposit()?
        T* GetRoom_ForDeposit();  //GetDepositHeadRoom()
        T* WithdrawTailElement();  
        T* GetWithdrawTailElement(bool takeout_from_queue);
        T* GetDepositHeadElement();  
    protected:
        // When you GetPreHeadObject(), you can view/check/modify it, also can ForwardHead(), equal to AppendObject().
 

        void _Init(const char* queue_name, int queue_size, T* first_element);
        // TODO:  T* to  T[]
        // void _Init(const char* queue_name, int queue_size, T[] all_elements);
        T* __all_elements;

    private:
        int __get_next_index(int current_index);
        int __get_previous_index(int current_index);
        int __queue_size = 0;
        int __element_size;   //todo: element_size
        int __deposit_push_head = 0;
        int __withdraw_pop_tail = 0;
        const char* __queue_name;

};


template <class T>
void QueueBase<T>::_Init(const char* queue_name, int queue_size, T* first_element){
    this->__queue_name = queue_name;
    this->__queue_size = queue_size; 
    this->__element_size = sizeof(T);
    this->__all_elements = first_element;
    // this->PrintOut("QueueBase:_Init()");
}

template <class T>
void QueueBase<T>::PrintOut(const char * title){
    Logger::Info(title);
    Logger::Print("__queue_name", this->__queue_name);
    Logger::Print("__queue_size",this->__queue_size);
    Logger::Print("__element_size",this->__element_size);
    Logger::Print("__withdraw_pop_tail", this->__withdraw_pop_tail);
    Logger::Print("__deposit_push_head", this->__deposit_push_head);
}

template <class T>
bool QueueBase<T>::Deposit(){
    // Logger::Debug("ommonQueue::Deposit()");
    // Logger::Print("__queue_name", this->__queue_name);
    // Logger::Print("old  __deposit_push_head ", this->__deposit_push_head);
    int next_head = this->__get_next_index(this->__deposit_push_head);
    
    if(next_head == this->__withdraw_pop_tail){
        Logger::Warn("QueueBase::Deposit() ");
        Serial.print("\n   Buffer is full");
        return true;
    }

    // new_object->DeepCopyTo((T*) (this->__all_elements + this->_push_head * this->_sizeof_item));

    this->__deposit_push_head = next_head;
    next_head = this->__get_next_index(this->__deposit_push_head);
    // Logger::Print("new  _push_head", this->__deposit_push_head);
    // this->PrintOut("caller is :  QueueBase::Deposit()");
    if (next_head == this->__withdraw_pop_tail)
        // buffer is full, after copying.
        return true;
    // buffer is NOT full. 
    return false;  
}

template <class T>
T* QueueBase<T>::GetRoom_ForDeposit(){
    // Logger::Debug("QueueBase::GetRoom_ForDeposit()");
    // T* head_message = this->__all_elements + __deposit_push_head * this->__element_size;
    T* head_message = this->__all_elements + __deposit_push_head ;
    // Serial.println(head_message->id);
    // Logger::Print("QueueBase::GetRoom_ForDeposit() point", 99);
    return  head_message;
}

template <class T>
T* QueueBase<T>::WithdrawTailElement(){
    T* tail_message = NULL;
    if (this->__deposit_push_head != this->__withdraw_pop_tail){
        // tail_message = this->__all_elements + this->__withdraw_pop_tail * this->__element_size;
        tail_message = this->__all_elements + this->__withdraw_pop_tail;
        this->__withdraw_pop_tail = this->__get_next_index(this->__withdraw_pop_tail);
    }
    return tail_message;
}

// Notice:  If buffer is empty, this will return unexpected result.
//TODO:  rename:  GetHeadElement
template <class T>
T* QueueBase<T>::GetDepositHeadElement(){
    int previous_head =  __get_previous_index(__deposit_push_head);
    // return  this->__all_elements + previous_head* this->__element_size;
    return  this->__all_elements + previous_head;
}

template <class T>
bool QueueBase<T>::BufferIsEmpty(){
    if (this->__deposit_push_head == this->__withdraw_pop_tail) 
        return true;
    return false;
}

template <class T>
bool QueueBase<T>::BufferIsFull(){
    int next_head = this->__get_next_index(this->__deposit_push_head);
    if (next_head == this->__withdraw_pop_tail)
        return true;
    return false;
}

template <class T>
int QueueBase<T>::GetFreeBuffersCount(){
    // Logger::Debug("QueueBase<T>::GetFreeBuffersCount()");
    int count = this->__deposit_push_head - this->__withdraw_pop_tail;
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

template <class T>
T* QueueBase<T>::GetWithdrawTailElement(bool takeout_from_queue){
    T* tail_message = NULL;
    if (this->__deposit_push_head != this->__withdraw_pop_tail){
        tail_message = this->__all_elements + this->__withdraw_pop_tail;
        if (takeout_from_queue){
            this->__withdraw_pop_tail = this->__get_next_index(this->__withdraw_pop_tail);
        }
    }
    return tail_message;
}
