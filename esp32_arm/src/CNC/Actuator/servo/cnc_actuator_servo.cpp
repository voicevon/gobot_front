#include "cnc_actuator_servo.h"


void CncActuatorServo::LinkServo(Servo* servo, bool is_inversed_dir){
    Logger::Debug("CncActuatorServo::LinkServo()");
    this->__servo = servo; 
    this->__inversed_dir = is_inversed_dir;
    this->__servo->write(40);   //TODO:  ??  remove this line.
    this->Init_FomularSlope(DEG_TO_RAD * 150);  //???
    this->ReInit_FormularOffset(DEG_TO_RAD * 150);  
}


void CncActuatorServo::SpinOnce(){
}

void CncActuatorServo::UpdateMovement(MoveBlock_SingleActuator* move){
    Logger::Debug("CncActuatorServo::UpdateMovement()");
    this->_target_position =  move->TargetPosition;   // TODO:  in base class?
    float physic_angle = this->GetPosition_FromCncUnit(move->TargetPosition);
    
    __servo->write(physic_angle);

    bool debug = true;
    if(debug){
        // float physic_angle = this->__ToServoDegree(this->_target_position);
        Logger::Print("current_cnc_position in degree = ", RAD_TO_DEG * this->_current_position);
        Logger::Print("target position in degree =  ",  this->_target_position);
        Logger::Print("target physical angle= ", physic_angle);
    }
}
