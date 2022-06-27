#include "mqtt_message_consumer.h"
#include <HardwareSerial.h>

bool MqttMessageConsumer::CheckMqttCommand(){
    // Serial.println("[Info] MqttMessageConsumer::CheckMqttCommand() is entering.");
    MessageQueue::SingleMessage* new_message = this->__mq->FetchTailMessage();
    if (new_message != NULL){
        // Serial.println("[Info] MqttMessageConsumer::CheckMqttCommand() got new meeage.");
        this->ExecuteMqttCommand(new_message->payload);
    }
}
