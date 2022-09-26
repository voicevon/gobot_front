

#include "common_queue.h"



class AnyClass: public Queue_able{
    public:
        int id;
        void DeepCopyTo(Queue_able* the_copy) override{
            AnyClass* copy=(AnyClass*)(the_copy);
            copy->id = this->id;
        };

};

class IamTester{
    void test(){
        CommonQueue queue;

        AnyClass xx;
        if(queue.GetFreeBuffersCount() > 5){
            queue.AppendObject(&xx);
        }
        if (! queue.BufferIsEmpty()){
            AnyClass* yy=(AnyClass*) queue.FetchTailObject();
        }
        

    };
};

