

#include "common_queue.h"



class AnyClass: public Queue_able{
    public:
        int id;
        void DeepCopyTo(AnyClass* the_copy) {
            AnyClass* copy=(AnyClass*)(the_copy);
            copy->id = this->id;
        };

};

class AnyQueue: public CommonQueue{
    public:
        AnyClass* Withdraw(){return (AnyClass*)this->_Withdraw(); };
        AnyClass* GetRoom() {return (AnyClass*)this->_GetRoom(); };
};

class IamTester{
    void test(){
        AnyQueue queue;
        if(queue.GetFreeBuffersCount() > 5){
            AnyClass* new_object = queue.GetRoom();
            new_object->id = 8;
            queue.Deposit();
        }
        if (! queue.BufferIsEmpty()){
            AnyClass* y=(AnyClass*) queue.Withdraw();
            AnyClass* yy = queue.Withdraw(); 
            yy= queue.GetRoom();

            // yy->DeepCopyTo(&xx);
        }
    };
};

