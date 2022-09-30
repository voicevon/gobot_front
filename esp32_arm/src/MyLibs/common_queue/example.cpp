

#include "common_queue.h"



class AnyClass: public Queue_able{
    public:
        int id;
        void DeepCopyTo(Queue_able* the_copy) override{
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

        AnyClass xx;
        if(queue.GetFreeBuffersCount() > 5){
            queue.AppendObject(&xx);
        }
        if (! queue.BufferIsEmpty()){
            AnyClass* y=(AnyClass*) queue.Withdraw();
            AnyClass* yy = queue.Withdraw(); 
            yy= queue.GetRoom();

            // yy->DeepCopyTo(&xx);
        }
    };
};

