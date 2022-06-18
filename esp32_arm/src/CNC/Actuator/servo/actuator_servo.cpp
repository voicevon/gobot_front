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
    //TODO: use timestamp
    return 0;
}


void ActuatorServo::MoveTo(bool is_absolute_position, float position_in_cnc_unit){
    float time;
    if (is_absolute_position){
        this->_target_cnc_position = position_in_cnc_unit;
    }else{
        this->_target_cnc_position += position_in_cnc_unit;
        time = abs(position_in_cnc_unit) / this->__speed_degree_per_second;
    }

    float physic_angle = this->__ToServoDegree(this->_target_cnc_position);
    this->__servo->write(physic_angle);
    
    if(this->__speed_degree_per_second >= 0){
        // for homing process of CNC.
        if (time < 50.0f / 1000) time =50;
        delay(time);
    }
    bool debug = false;
    if(debug){
        Serial.print("[debug] ActuatorServo::MoveTo() cnc_position in degree = ");
        Serial.print(RAD_TO_DEG * position_in_cnc_unit);
        Serial.print("   ");
        Serial.print(RAD_TO_DEG * this->__current_position_in_rad);
        Serial.print("    physical angle= ");
        Serial.println(physic_angle);
    }
    
    this->__current_position_in_rad = this->_target_cnc_position;    // TODO: record the timestep, To get distance to target by time.
}

void ActuatorServo::SetPosition(float cnc_position_in_rad){
    // This function is called from CNC, Who send a rad-angle of position's unit. 
    int8_t dir = 1;
    if (this->__inversed_dir) dir = -1;
    this->__current_position_in_rad = cnc_position_in_rad;
    this->__position_offset_in_rad = cnc_position_in_rad - DEG_TO_RAD * this->__servo->read() * dir;
    bool debug=true;
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