#include "actuator_dc_motor.h"
#include "Arduino.h"
#include "MyBoards/board_base.h"
#include "CNC/gcode/line_segment_queue.h"   //TODO:  use block


#define INERTIA_DISTANCE  0.032    // this is a CNC unit  0.016== (1/386)/ (2*PI), around 12.7mm



void ActuatorDcMotor::SpinOnce(){
    this->__sensor->GetRawSensor()->update();
    // real speed control, position check, auto stop....
    float abs_distance_to_target = this->GetAbsDistanceToTarget_InCncUnit();
    bool serial_output = false;
    if(abs_distance_to_target < INERTIA_DISTANCE){
    // if (false){
        // The wheel will continue to run a short time after stoping, because the inertia.
        // TDDO:  How to deal with negtive distance?
        this->__h_bridge->Stop();
    }else{
        this->__count_down_for_serial_print--;
        if (this->__count_down_for_serial_print <=0){
            // Logger::Print("Abs DIstanceToTarget in CNC Rad ", abs_distance);
            this->__count_down_for_serial_print = 200;
            this->__count_down_for_serial_print = 9900;
            serial_output = true;
        }

        // speed pid 
        float speed_error =  abs(this->__sensor->GetCurrentVelocity()) - abs(this->__target_velocity);
        float pwm_speed =  - this->__speed_pid->FeedError(speed_error) * 8;

        if (serial_output){
            // Serial.print("velocity of sensor, speed_error, pwm_speed \t");
            // Logger::Debug("velocity", velocity_in_cnc_unit);
            // Logger::Debug("target_speed", )
            Serial.print("\n    pos(cur,dis): ");
            Serial.print(this->__sensor->GetCurrentPosition());
            Serial.print("\t");
            Serial.print(abs_distance_to_target);
            Serial.print("   speed(tar,cur,err,pwm): ");
            Serial.print(this->__target_velocity);
            Serial.print("\t");
            Serial.print(this->__sensor->GetCurrentVelocity());
            Serial.print("\t");
            Serial.print(speed_error);
            Serial.print("\t");
            Serial.print(pwm_speed);
            Serial.print("        pid: ");
            Serial.print(this->__speed_pid->P);
            Serial.print("\t");
            Serial.print(this->__speed_pid->I);
            Serial.print("\t");
            Serial.print(this->__speed_pid->D);
        }
        // todo:  pwm_speed = ax^3 + bx^2 + cx + f
        bool dir_is_cw = (this->__target_velocity > 0);
        // dir_is_cw = true;
        // pwm_speed = constrain(pwm_speed, 0 , 255);
        if (pwm_speed > 255.0f) pwm_speed = 255.0f;
        if (pwm_speed < 30.0f) pwm_speed = 0.0f;
        this->__h_bridge->SetPwmSpeed(dir_is_cw, pwm_speed);
        
        // this->SetPwmSpeed(dir_is_cw, pwm_speed);
    }
}

float ActuatorDcMotor::GetCurrentPosition(){
    // from sensor_angle to cnc_angle.
    // cnc_angle == sensor_angle  * (10 / 10) * (10 / 56) * (56 / 157) 
    //           == sensor_angle * (SENSOR_GEAR_COUNT / MOTOR_GEAR_COUNT)* (MOTOR_GEAR_COUNT / DRIVER_GEAR_COUNT) * (DRIVER_GEAR_COUNT / CHAIN_PITCH_COUNT)
    //           == sensor_angle * (SENSOR_GEAR_COUNT / CHAIN_PITCH_COUNT)

    return this->__sensor->GetCurrentPosition();
}

void ActuatorDcMotor::SetCurrentPositionAs(float position_in_cnc_unit){
    this->__sensor->SetCurrentPosition(position_in_cnc_unit);
}

// void ActuatorDcMotor::SetTargetPositionTo(bool is_absolute_position, float target_position){
void ActuatorDcMotor::UpdateMovement(LineSegment* move){

    Logger::Debug("ActuatorDcMotor::SetTargetPositionTo()  is entering");
    Logger::Print("is_absolute_position", move->IsAbsTargetPosition);
    Logger::Print("target_position", move->TargetPosition);
    Logger::Print("speed", move->Speed);
    
    
    // if (is_absolute_position){
    if (move->IsAbsTargetPosition){
        // this->_target_cnc_position = target_position;
        this->_target_cnc_position = move->TargetPosition;
    }else{
        // this->_target_cnc_position = this->GetCurrentPosition() + target_position;
        this->_target_cnc_position = this->GetCurrentPosition() + move->TargetPosition;
    }

    this->__target_velocity =  move->Speed;
    if (this->_target_cnc_position < this->GetCurrentPosition()){
        this->__target_velocity = - move->Speed;
    }

    Logger::Print("-------------------------------------",0);
    Logger::Print("Current_position",this->GetCurrentPosition());
    Logger::Print("this->_target_cnc_position", this->_target_cnc_position);
    Logger::Print("target_velocity", this->__target_velocity);
}

float ActuatorDcMotor::GetAbsDistanceToTarget_InCncUnit(){
    // sensor --> current poistion   --> distance to target
    // TODO:  minus distance.
    // Logger::Debug(" ActuatorDcMotor::GetAbsDistanceToTarget_InCncUnit()");
    // Logger::Print("target_cnc_position", this->_target_cnc_position);
    // Logger::Print("Current_position", this->GetCurrentCncPosition());
    return abs(this->_target_cnc_position - this->GetCurrentPosition());
}



// void ActuatorDcMotor::SetSpeed(float rad_per_second){
//     this->__target_speed = rad_per_second;
// }

void ActuatorDcMotor::SetAccelleration(float accelleration_in_cnc_unit){
    // this is a future feature.
}


void ActuatorDcMotor::ForceStop(){   
    //* Only G28 is using this.
    //* SpinOnce will restart the movement, untill the actuator reach targetPosition.
    Logger::Debug("ActuatorDcMotor::ForceStop() is entering...");
    this->__h_bridge->Stop();
}

void ActuatorDcMotor::UpdateTargetPositionFromCurrent(){
    Logger::Debug("ActuatorDcMotor::UpdateTargetPositionFromCurrent() is entering...");
    this->_target_cnc_position = this->GetCurrentPosition();
}






void ActuatorDcMotor::PrintOut(){
    Logger::Debug("ActuatorDcMotor::PrintOut()");
    // Logger::Print("this->__pwm_channel ",this->__pwm_channel );
    // Logger::Print("this->__h_bridge_pin_dir ",this->__h_bridge_pin_dir );
    // Logger::Print("this->__h_bridge_pin_speed ",this->__h_bridge_pin_speed );
    Serial.println(FCBC_RESET);
}



void ActuatorDcMotor::Test_PwmSpeed(bool dir_is_cw,  uint32_t pwm_speed){
    this->__h_bridge->SetPwmSpeed(dir_is_cw, pwm_speed);
}
