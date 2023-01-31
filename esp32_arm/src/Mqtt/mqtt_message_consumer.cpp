#include "mqtt_message_consumer.h"
#include <HardwareSerial.h>

bool MqttMessageConsumer::CheckMqttCommand(){
    // Logger::Info(" MqttMessageConsumer::CheckMqttCommand()");
    MessageQueue::SingleMessage* new_message = __mq->FetchTailMessage(true);
    if (new_message != NULL){
        // Logger::Print("MqttMessageConsumer::CheckMqttCommand() got new meeage.", new_message->payload);
        onGot_MqttMessage(new_message->payload);
    }
}
