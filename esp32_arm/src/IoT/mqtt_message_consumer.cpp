#include "mqtt_message_consumer.h"


bool MqttMessageConsumer::CheckMqttCommand(){
    MessageQueue::SingleMessage* new_message = this->__mq->FetchTailMessage();
    // if (new_message != NULL){
    //     this->ExecuteMqttCommand(new_message.payload);
    // }
}
