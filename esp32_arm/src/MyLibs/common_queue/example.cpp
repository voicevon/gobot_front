

#include "common_queue.h"



class ToBeInQueue: public Queue_able{
    public:
        int id;
        void DeepCopyTo(Queue_able* the_copy) override{
            ToBeInQueue* copy=(ToBeInQueue*)(the_copy);
            copy->id = this->id;
        };

};

class IamTester{
    void test(){
        CommonQueue queue;

        ToBeInQueue xx;
        if(queue.GetFreeBuffersCount() > 5){
            queue.AppendObject(&xx);
        }
        if (! queue.BufferIsEmpty()){
            ToBeInQueue* yy=(ToBeInQueue*) queue.FetchTailObject();
        }
        

    };
};

