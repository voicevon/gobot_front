#include "cnc_actuator_servo.h"


void CncActuatorServo::LinkServo(Servo* servo, bool is_inversed_dir){
    Logger::Debug("CncActuatorServo::LinkServo()");
    this->__servo = servo; 
    this->Formular_SetSlope(DEG_TO_RAD * 150);  //???
    this->Formular_SetRawOffset(DEG_TO_RAD * 150);  
}


void CncActuatorServo::SpinOnce(){
    _is_moving = false;
}

void CncActuatorServo::UpdateMovement(MoveBlock_SingleActuator* move){
    Logger::Debug("CncActuatorServo::UpdateMovement()");
    this->_target_position =  move->TargetPosition;   // TODO:  in base class?
    float physic_angle = this->GetPosition_FromCncUnit(move->TargetPosition);
    __servo->write(physic_angle);

    bool debug = true;
    if(debug){
        Logger::Print("current_cnc_position in degree = ", RAD_TO_DEG * this->_current_position);
        Logger::Print("target position in degree =  ",  this->_target_position);
        Logger::Print("target physical angle= ", physic_angle);
    }
}
