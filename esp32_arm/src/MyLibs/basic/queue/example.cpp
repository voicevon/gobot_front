// #include "queue_base.h"


// class AnyClass: public Queue_able{
//     public:
//         int id;
//         void DeepCopyTo(AnyClass* the_copy) {
//             AnyClass* copy = the_copy;
//             copy->id = this->id;
//         };
// };


// class AnyQueue: public QueueBase<AnyClass>{
//     public:
//         AnyClass* Withdraw(){return this->_Withdraw(); };
//         AnyClass* GetRoom() {return this->_GetRoom(); };
// };


// class IamTester{
//     void test(){
//         AnyQueue queue;
//         if(queue.GetFreeBuffersCount() > 5){
//             AnyClass* new_object = queue.GetRoom();
//             new_object->id = 8;
//             queue.Deposit();
//         }
//         if (! queue.BufferIsEmpty()){
//             AnyClass* y= queue.Withdraw();
//             AnyClass* yy = queue.Withdraw(); 
//             yy= queue.GetRoom();
//             Serial.println(yy->id);
//             Serial.println(y->id);

//             // yy->DeepCopyTo(&xx);
//         }
//     };
// };

