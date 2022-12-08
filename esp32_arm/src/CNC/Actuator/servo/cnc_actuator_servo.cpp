#include "cnc_actuator_servo.h"

void CncActuatorServo::LinkServo(Servo* servo){
    // Logger::Debug("CncActuatorServo::LinkServo()");
    this->__servo = servo; 
}

void CncActuatorServo::SpinOnce(){
    _is_moving = false;
}

void CncActuatorServo::SetCurrentPositionAs(float new_position){
    //Do nothing.  arm_solution will use the current position. Servo will take no effection.
}

float CncActuatorServo::GetCurrentPosition(){
    return __target_position;
}


void CncActuatorServo::UpdateMovement(MoveBlock_SingleActuator* move){
    Logger::Debug("CncActuatorServo::UpdateMovement()");
    __servo->write(move->TargetPosition);
    __target_position = move->TargetPosition;

    _is_moving = true;

    // Logger::Print("current_cnc_position = ", this->_current_position);
    // Logger::Print("target position in degree =  ",  this->_target_position);
    // Logger::Print("target physical angle= ", physic_angle);
}
