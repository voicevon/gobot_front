#include "actuator_servo.h"


void ActuatorServo::LinkServo(Servo* servo, bool is_inversed_dir){
    this->__servo = servo; 
    this->__inversed_dir = is_inversed_dir;
    this->__servo->write(40);
    this->SetPosition(DEG_TO_RAD * 150); 
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

float ActuatorServo::GetCurrentPosition_InCncUnit(){
    return this->__current_position_in_rad;
}

float ActuatorServo::GetDistanceToTarget_InCncUnit(){
    return this->__distance_to_target_in_rad;
}

void ActuatorServo::SpinOnce(){
    // Execute moving to target position,  and follow target speed
    int64_t  now = esp_timer_get_time();
    int64_t time_interval_in_us = (now - this->last_spin_timestamp) ;
    if(time_interval_in_us < (60 *1000)) return;   // at least 60ms
    
    this->__distance_to_target_in_rad = this->_target_cnc_position - this->__current_position_in_rad;
    float distance_should_be_moved = DEG_TO_RAD * this->__speed_degree_per_second * time_interval_in_us /1000000L;
    if (distance_should_be_moved <= this->__distance_to_target_in_rad){
        // after running this step, will not go over the target position.
        this->__current_position_in_rad += distance_should_be_moved;
        float servo_angle_in_degree = this->__ToServoDegree(this->__current_position_in_rad);
        this->__servo->write(servo_angle_in_degree);
        this->last_spin_timestamp = now;
        this->__is_moving = true;
    }else{
        //Almost arrived target position already.
        this->__is_moving = false;
    }
}

void ActuatorServo::MoveTo(bool is_absolute_position, float position_in_cnc_unit){
    // float time;


    if (is_absolute_position){
        this->_target_cnc_position = position_in_cnc_unit;
    }else{
        this->_target_cnc_position += position_in_cnc_unit;
        // time = abs(position_in_cnc_unit) / this->__speed_degree_per_second;
    }

    if (this->_target_cnc_position > this->__current_position_in_rad){
        this->__moving_direction = 1;
    }else{
        this->__moving_direction = -1;
    }
    // if(this->__speed_degree_per_second >= 0){
    //     // for homing process of CNC.
    //     if (time < 50.0f / 1000) time =50;
    //     delay(time);
    // }
    bool debug = false;
    if(debug){
        Serial.print("[debug] ActuatorServo::MoveTo() cnc_position in degree = ");
        Serial.print(RAD_TO_DEG * position_in_cnc_unit);
        Serial.print("   ");
        Serial.print(RAD_TO_DEG * this->__current_position_in_rad);
        Serial.print("    physical angle= ");
        float physic_angle = this->__ToServoDegree(this->_target_cnc_position);
        Serial.println(physic_angle);
    }
    
    this->__current_position_in_rad = this->_target_cnc_position;    // TODO: record the timestep, To get distance to target by time.
    this->last_spin_timestamp = esp_timer_get_time();
    this->SpinOnce();
}

void ActuatorServo::SetPosition(float cnc_position_in_rad){
    // This function is called from CNC, Who send a rad-angle of position's unit. 
    int8_t dir = 1;
    if (this->__inversed_dir) dir = -1;
    this->__current_position_in_rad = cnc_position_in_rad;
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

void ActuatorServo::SetSpeed(float speed_per_second){
    this->__speed_degree_per_second = RAD_TO_DEG * speed_per_second; 
}




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