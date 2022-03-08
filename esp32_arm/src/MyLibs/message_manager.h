#include "message_queue.h"
#include "message_consumer.h"



class MessageManager{
    public:
        void SpinOnce(){
            this->_consumer->SpinOnce();
        }
        ;
        void AppendMessage(){
            this->_mq->AppendMessage();
        };

    private:
        MessageConsumer* _consumer;
        MessageQueue* _mq;


};