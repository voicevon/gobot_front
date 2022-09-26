

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
        AnyClass* FetchTailAnyClass(){return (AnyClass*)this->FetchTailObject(); };
        AnyClass* GetHeadAnyClass() {return (AnyClass*)this->GetHeadObject(); };
};

class IamTester{
    void test(){
        AnyQueue queue;

        AnyClass xx;
        if(queue.GetFreeBuffersCount() > 5){
            queue.AppendObject(&xx);
        }
        if (! queue.BufferIsEmpty()){
            AnyClass* y=(AnyClass*) queue.FetchTailObject();
            AnyClass* yy = queue.FetchTailAnyClass(); 
            yy= queue.GetHeadAnyClass();
        }
        

    };
};

