#include "pid_controllers.h"
#include "MyLibs/basic/logger.h"

PidControllers::PidControllers(uint8_t pid_controller_count){
    this->__max_controller_index = pid_controller_count;
}

void PidControllers::AppendPidController(PIDController* new_pid_controller){
    this->__all_pid_controllers[this->__next_controller_index] = new_pid_controller;
    this->__next_controller_index++;
    if (this->__next_controller_index > this->__max_controller_index){
        Logger::Error("PidControllers::AppendPidController()   TOO MANY CONTROLLERS INSIDE!");
    }
}

// void PidControllers::Set_P(uint8_t index, float f_value){
//     this->__all_pid_controllers[index]->P = f_value;
// }

