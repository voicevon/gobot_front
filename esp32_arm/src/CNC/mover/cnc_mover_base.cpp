#include "cnc_mover_base.h"


// bool MotorIsMoving(char moto_name);


void CncMoverBase::SetActuatorSpeed(EnumAxis actuator_name, float steps_per_second){
    if (actuator_name ==AXIS_ALPHA){
        this->_actuator_alpha_base->SetSpeed(steps_per_second);
    }else if(actuator_name == AXIS_BETA){
        this->_actuator_beta_base->SetSpeed(steps_per_second);
    }else{
        // Logger::Halt()
        Serial.println("[Error] CncMoverBase::SetActuatorSpeed() ");
    }
}