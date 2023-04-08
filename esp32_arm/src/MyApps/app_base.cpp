#include "app_base.h"

void AppBase::SpinOnce(){
    if (this->test_id != 1234){
        Logger::Warn("AppBase::SpinOnce(),  The bug is eating.!!!!");
        Logger::Halt("BUG!");
    }
    // Logger::Debug("AppBase::SpinOnce()");
    // Logger::Print("this->test_id", this->test_id);
    // this->_gcode_queue->SayHello("caller is :  AppBase::SpinOnce()");
    if (!this->_gcode_queue->BufferIsFull()){
        // Logger::Print("AppBase::SpinOnce()   point", 1);
        this->CheckMqttCommand();
        // Logger::Print("AppBase::SpinOnce()   point", 99);
    }
}



