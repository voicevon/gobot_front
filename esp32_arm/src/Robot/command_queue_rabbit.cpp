#include "command_queue_rabbit.h"

void CommandQueueRabbit::LinkLocalCommandQueue(GcodeQueue* loacalMQ){
    this->__localMQ = loacalMQ;
}

void CommandQueueRabbit::SubscribeMqtt(AsyncMqttClient* mqttClient, const char* main_topic, const char* feedback_topic){
    this->__mqttClient = mqttClient;
    mqttClient->subscribe(main_topic, 2);
    // mqttClient->subscribe("gobot/x2134/house", 2);
    this->topic_feedback = String(feedback_topic);
    // this->topic_feedback = "gobot/x2134/house/fb";

    bool debug=true;
    if(debug){
        Serial.print("\nCommandQueueRabbit::SubscribeMqtt() is done    ");
        Serial.print(main_topic);
        Serial.print("      ");
        Serial.println(feedback_topic);
    }
}


void CommandQueueRabbit::OnReceived(const char* payload, int length){
    // Put message to local MQ   
    // TODO:: Is this necessary??
    char* p = (char*)(payload) + length;
    *p = 0x00;

    this->__local_mq_is_full = this->__localMQ->AppendGcodeCommand(payload, length); 

    // send message to feedback topic
    if (this->__local_mq_is_full){
        Serial.print("\nCommandQueueRabbit::OnReceived() is full");
        return;
    }
    this->__mqttClient->publish(this->topic_feedback.c_str(), 2, true, payload, length);
}

// This function will be invoked on master thread.
void CommandQueueRabbit::SpinOnce(){
    if (this->__local_mq_is_full)
        if (! this->__localMQ->BufferIsFull()){
            Serial.println("\n                  CommandQueueRabbit::SpinOnce() local mq got a free room");
            MessageQueue::SingleMessage* pMessage = this->__localMQ->GetHeadMessage();
            this->__mqttClient->publish(this->topic_feedback.c_str(), 2, true, pMessage->payload, pMessage->length);
            this->__local_mq_is_full = false;
        }

}