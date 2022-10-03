#include "actuator_servo.h"
// #include "CNC/gcode/line_segment_queue.h"   //TODO:  use block


void ActuatorServo::LinkServo(Servo* servo, bool is_inversed_dir){
    this->__servo = servo; 
    this->__inversed_dir = is_inversed_dir;
    this->__servo->write(40);
    this->InitFormular_FromCncPosition(DEG_TO_RAD * 150); 
    bool debug = false;
    if (debug){
        float verify_cnc_position = this->__ToCncRad(50);
        Serial.print("[Debug] ActuatorServo::LinkServo()   verify ");
        Serial.println(RAD_TO_DEG * verify_cnc_position);
        float verify_servo_position = this->__ToServoDegree(DEG_TO_RAD * 140);
        Serial.print("[Debug] ActuatorServo::LinkServo()   verify ");
        Serial.println(verify_servo_position);
    }
}

// float ActuatorServo::GetAbsDistanceToTarget_InCncUnit(){
//     return abs(this->_target_position - this->_current_position);
// }

void ActuatorServo::SpinOnce(){
    // Execute moving to target position,  and follow target speed
    // Never know the current position. Shold be equal to target_position.  Right?  TODO:  doble check.
    if (!this->__is_moving) return;

    int64_t  now = esp_timer_get_time();
    int64_t time_interval_in_us = (now - this->__last_spin_timestamp) ;
    bool debug = false;
    if (debug){
        Serial.print("[Debug] ActuatorServo::SpinOnce() now= ");
        Serial.print(now);
        Serial.print("   interval us = ");
        Serial.println(time_interval_in_us);
    }
    if(time_interval_in_us < (60L * 1000)) return;   // at least 60ms

    float distance_to_target_in_rad = abs (this->_target_position - this->_current_position);
    float distance_should_be_moved = abs(DEG_TO_RAD * this->__speed_degree_per_second * time_interval_in_us /1000000L);
    

    if (distance_should_be_moved <= distance_to_target_in_rad){
        // after running this step, The servo will not go over the target position.
        this->_current_position += distance_should_be_moved * this->__moving_direction_of_cnc;
        this->__last_spin_timestamp = now;
    }else{
        //Almost arrived target position already, So this is the last step.
        this->_current_position = this->_target_position;
        this->__is_moving = false;
    }
    float servo_angle_in_degree = this->__ToServoDegree(this->_current_position);
    debug = false;
    if(debug){
        Serial.print("time_interval_in_us= ");
        Serial.print(time_interval_in_us);
        Serial.print("  distance_to_target= ");
        Serial.print(RAD_TO_DEG* distance_to_target_in_rad);
        Serial.print("  speed= ");
        Serial.print(this->__speed_degree_per_second);
        Serial.print("  current position= ");
        Serial.print(RAD_TO_DEG * this->_current_position);
        Serial.print("  distance_should_be_moved=");
        Serial.print(RAD_TO_DEG* distance_should_be_moved);
        Serial.print(" direction= ");
        Serial.print(this->__moving_direction_of_cnc);
        Serial.print(" servo_angle= ");
        Serial.println(servo_angle_in_degree);
    }
    this->__servo->write(servo_angle_in_degree);
}

// void ActuatorServo::SetTargetPositionTo(bool is_absolute_position, float position_in_cnc_unit){
void ActuatorServo::UpdateMovement(MoveBlock_SingleActuator* move){

    // if (is_absolute_position){
    if (move->IsAbsTargetPosition){
        // this->_target_position = position_in_cnc_unit;
        this->_target_position = move->TargetPosition;
    }else{
        // this->_target_position = this->_current_position + position_in_cnc_unit;
        this->_target_position = this->_current_position + move->TargetPosition;
    }

    this->__moving_direction_of_cnc = 1;
    if (this->_target_position < this->_current_position){
        this->__moving_direction_of_cnc = -1;
    }

    bool debug = false;
    if(debug){
        Serial.print("[debug] ActuatorServo::MoveTo() current_cnc_position in degree = ");
        Serial.print(RAD_TO_DEG * this->_current_position);
        Serial.print("  target position=  ");
        Serial.print(RAD_TO_DEG * this->_target_position);
        Serial.print("    target physical angle= ");
        float physic_angle = this->__ToServoDegree(this->_target_position);
        Serial.println(physic_angle);
    }

}

void ActuatorServo::StartToMove(){
    this->__is_moving = true;   //??
    this->__last_spin_timestamp = esp_timer_get_time();
}

void ActuatorServo::InitFormular_FromCncPosition(float cnc_position_in_rad){
    // This function is called from CNC, Who send a rad-angle in unit. 
    int8_t dir = 1;
    if (this->__inversed_dir) dir = -1;
    this->_current_position = cnc_position_in_rad;
    this->__position_offset_in_rad = cnc_position_in_rad - DEG_TO_RAD * this->__servo->read() * dir;
    bool debug = false;
    if(debug){
        Serial.print("[Debug] ActuatorServo::SetPosition()  cnc_position, servo_position, __position_offset in_degree = ");
        Serial.print(RAD_TO_DEG * cnc_position_in_rad);
        Serial.print (",   " );
        Serial.print(this->__servo->read());
        Serial.print(",   ");
        Serial.println(RAD_TO_DEG * this->__position_offset_in_rad);
    }
}

// void ActuatorServo::SetSpeed(float speed_in_cnc_unit){
//     this->__speed_degree_per_second = abs(RAD_TO_DEG * speed_in_cnc_unit); 
// }


float ActuatorServo::__ToServoDegree(float from_cnc_rad){
    int8_t dir = 1;
    if (this->__inversed_dir) dir = -1;
    return  RAD_TO_DEG * (from_cnc_rad * dir + this->__position_offset_in_rad);
}

float ActuatorServo::__ToCncRad(float from_servo_degree){
    int8_t dir = 1;
    if (this->__inversed_dir) dir = -1;
    return  from_servo_degree * DEG_TO_RAD * dir + this->__position_offset_in_rad;
}