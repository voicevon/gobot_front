#include "cnc_app_base.h"


void CncAppBase::onGot_MqttMessage(const char* payload, uint16_t payload_len){
	this->gcode_queue.AppendGcodeCommand(payload);
}

void CncAppBase::SpinOnce(){
    if (this->test_id != 1234){
        Logger::Warn("CncAppBase::SpinOnce(),  The bug is eating.!!!!");
        Logger::Halt("BUG!");
    }
    // Logger::Debug("CncAppBase::SpinOnce()");
    // Logger::Print("this->test_id", this->test_id);
    // this->gcode_queue.SayHello("caller is :  CncAppBase::SpinOnce()");
    if (!this->gcode_queue.BufferIsFull()){
        // Logger::Print("CncAppBase::SpinOnce()   point", 1);
        this->CheckMqttCommand();
        // Logger::Print("CncAppBase::SpinOnce()   point", 99);
    }
}

bool CncAppBase::CheckMqttCommand(){
    // Logger::Info(" MqttMessageConsumer::CheckMqttCommand()");
    // MqttMessage* new_message = __mq->FetchTailMessage(true);
    // if (new_message != NULL){
    //     // Logger::Print("MqttMessageConsumer::CheckMqttCommand() got new meeage.", new_message->payload);
    //     onGot_MqttMessage(new_message->payload);
    // }
}


