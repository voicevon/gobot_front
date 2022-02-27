#include "command_queue_rabbit.h"

void CommandQueueRabbit::Init(CommandQueue* loacalMQ, String mqtt_topic){
    this->__localMQ = loacalMQ;
}

void CommandQueueRabbit::OnReceived(byte* payload){
    // Put message to local MQ
    this->__localMQ->AppendGcodeCommand("");
    // send message to feedback topic
    // mqtt.send(this->topic_feedback, payload);
}