#include "cnc_actuator_servo.h"


void CncActuatorServo::LinkServo(Servo* servo, bool is_inversed_dir){
    Logger::Debug("CncActuatorServo::LinkServo()");
    this->__servo = servo; 
    this->__inversed_dir = is_inversed_dir;
    this->__servo->write(40);   //??
    // this->InitFormular_FromCncPosition(DEG_TO_RAD * 150); 
    this->Init_FomularSlope(DEG_TO_RAD * 150);  //???
    this->ReInit_FormularOffset(DEG_TO_RAD * 150);  
    // bool debug = false;
    // if (debug){
    //     float verify_cnc_position = this->__ToCncRad(50);
    //     Serial.print("[Debug] CncActuatorServo::LinkServo()   verify ");
    //     Serial.println(RAD_TO_DEG * verify_cnc_position);
    //     float verify_servo_position = this->__ToServoDegree(DEG_TO_RAD * 140);
    //     Serial.print("[Debug] CncActuatorServo::LinkServo()   verify ");
    //     Serial.println(verify_servo_position);
    // }
}


void CncActuatorServo::SpinOnce(){
    // Execute moving to target position,  and follow target speed
    // Never know the current position. Shold be equal to target_position.  Right?  TODO:  doble check.


    // if (!this->__is_moving) return;
    // Logger::Debug("CncActuatorServo::SpinOnce()");
    // int64_t  now = esp_timer_get_time();
    // int64_t time_interval_in_us = (now - this->__last_spin_timestamp) ;
    // bool debug = false;
    // if (debug){
    //     Serial.print("[Debug] CncActuatorServo::SpinOnce() now= ");
    //     Serial.print(now);
    //     Serial.print("   interval us = ");
    //     Serial.println(time_interval_in_us);
    // }
    // if(time_interval_in_us < (60L * 1000)) return;   // at least 60ms

    // float distance_to_target_in_rad = abs (this->_target_position - this->_current_position);
    // float distance_should_be_moved = abs(DEG_TO_RAD * this->__speed_degree_per_second * time_interval_in_us /1000000L);
    

    // if (distance_should_be_moved <= distance_to_target_in_rad){
    //     // after running this step, The servo will not go over the target position.
    //     this->_current_position += distance_should_be_moved * this->__moving_direction_of_cnc;
    //     this->__last_spin_timestamp = now;
    // }else{
    //     //Almost arrived target position already, So this is the last step.
    //     this->_current_position = this->_target_position;
    //     this->__is_moving = false;
    // }
    // float servo_angle_in_degree = this->__ToServoDegree(this->_current_position);
    // debug = false;
    // if(debug){
    //     Logger::Print("time_interval_in_us= ", time_interval_in_us);
    //     Logger::Print("  distance_to_target= ", RAD_TO_DEG* distance_to_target_in_rad);
    //     Logger::Print("  speed= ", this->__speed_degree_per_second);
    //     Logger::Print("  current position= ", RAD_TO_DEG * this->_current_position);
    //     Logger::Print("  distance_should_be_moved=", RAD_TO_DEG* distance_should_be_moved);
    //     Logger::Print(" direction= ",this->__moving_direction_of_cnc);
    //     Logger::Print(" servo_angle= ", servo_angle_in_degree);
    // }
    // this->__servo->write(servo_angle_in_degree);
}

void CncActuatorServo::UpdateMovement(MoveBlock_SingleActuator* move){
    Logger::Debug("CncActuatorServo::UpdateMovement()");
    this->_target_position =  move->TargetPosition;   // TODO:  in base class?
    float physic_angle = this->GetPosition_FromCncUnit(move->TargetPosition);
    
    __servo->write(physic_angle);
    // this->__moving_direction_of_cnc = 1;
    // if (this->_target_position < this->_current_position){
    //     this->__moving_direction_of_cnc = -1;
    // }

    bool debug = true;
    if(debug){
        // float physic_angle = this->__ToServoDegree(this->_target_position);
        Logger::Print("current_cnc_position in degree = ", RAD_TO_DEG * this->_current_position);
        Logger::Print("target position in degree =  ",  this->_target_position);
        Logger::Print("target physical angle= ", physic_angle);
    }

}

// void CncActuatorServo::StartToMove(){
//     this->__is_moving = true;   //??
//     this->__last_spin_timestamp = esp_timer_get_time();
// }

// float CncActuatorServo::__ToServoDegree(float from_cnc_rad){
//     // int8_t dir = 1;
//     // if (this->__inversed_dir) dir = -1;
//     // return  RAD_TO_DEG * (from_cnc_rad * dir + this->__position_offset_in_rad);
// }

// float CncActuatorServo::__ToCncRad(float from_servo_degree){
//     int8_t dir = 1;
//     if (this->__inversed_dir) dir = -1;
//     return  from_servo_degree * DEG_TO_RAD * dir + this->__position_offset_in_rad;
// }