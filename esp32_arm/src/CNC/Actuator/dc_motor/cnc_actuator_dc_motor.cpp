#include "cnc_actuator_dc_motor.h"
#include "Arduino.h"
#include "MyBoards/board_base.h"   //todo: ??


#define INERTIA_DISTANCE  0.064    // this is a CNC unit  0.016== (1/386)/ (2*PI), around 12.7mm

// real speed control, position check, auto stop....
void CncActuatorDcMotor::__SpinOnce_FollowVelocity(float velocity){
    static uint32_t last_micros;
    bool serial_output = false;
    if(micros() - last_micros > 1000){
        // Logger::Print("Abs DIstanceToTarget in CNC Rad ", abs_distance);
        serial_output = true;
        last_micros = micros();
    }

    // speed pid 
    float speed_error =  abs(this->__encoder->getVelocity()) - abs(this->__target_velocity);
    float pwm_speed =  - this->__speed_pid->FeedError(speed_error) * 8;

    if (serial_output){
        // Serial.print("velocity of sensor, speed_error, pwm_speed \t");
        // Logger::Debug("velocity", velocity_in_cnc_unit);
        // Logger::Debug("target_speed", )
        Serial.print("\n    pos(cur,dis): ");
        Serial.print(this->_current_position);
        Serial.print("\t");
        Serial.print(this->_target_position - this->_current_position);
        Serial.print("   speed(tar,cur,err,pwm): ");
        Serial.print(this->__target_velocity);
        Serial.print("\t");
        // Serial.print(this->__sensor->GetCurrentVelocity());
        Serial.print(this->__encoder->getVelocity());
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
    // pwm_speed = constrain(pwm_speed, 0 , 255);
    if (pwm_speed > 255.0f) pwm_speed = 255.0f;
    if (pwm_speed < 30.0f) pwm_speed = 0.0f;
    this->__h_bridge->SetPwmSpeed(dir_is_cw, pwm_speed);
}    


void CncActuatorDcMotor::SpinOnce(){
    // Logger::Debug("CncActuatorDcMotor::SpinOnce()");
    this->__encoder->update();
    this->_current_position = this->__encoder->getSensorAngle();

    if (this->__is_moving){
        float abs_distance_to_target = abs (this->_target_position-this->_current_position);
        if(abs_distance_to_target < INERTIA_DISTANCE){
            // The wheel will continue to run a short time after stoping, because the inertia.
            // TDDO:  How to deal with negtive distance?
            this->__h_bridge->Stop();
            this->__is_moving= false;
        } else{
            this->__SpinOnce_FollowVelocity(this->__target_velocity);
        } 
    }
}

void CncActuatorDcMotor::UpdateMovement(MoveBlock_SingleActuator* move){

    Logger::Debug("CncActuatorDcMotor::UpdateMovement()");
    Logger::Print("target_position", move->TargetPosition);
    Logger::Print("speed", move->Speed);
    
    this->_target_position = this->GetPosition_FromCncUnit(move->TargetPosition);
   

    // TODO:  got target speed, not target_velocity. But:
    // seems, target_speed will be used directly,  target_velocicy will not be. 
    this->__target_velocity = this->GetSpeed_FromCncUnit(move->Speed);

    //actuator_dc_motor speed, comes from encoder, speed unit is: rad_per_second.
    //CNC velocity unit is: ??
    this->__target_velocity =  move->Speed;

    if (this->_target_position < this->GetCurrentPosition()){
        this->__target_velocity = - move->Speed;
    }
    this->__is_moving = true;

    Logger::Print("-------------------------------------",0);
    Logger::Print("Current_position",this->GetCurrentPosition());
    Logger::Print("this->_target_position", this->_target_position);
    Logger::Print("target_velocity", this->__target_velocity);
}

void CncActuatorDcMotor::ForceStop(){   
    //* Only G28 is using this.
    Logger::Debug("CncActuatorDcMotor::ForceStop() is entering...");
    this->__h_bridge->Stop();
    this->__is_moving = false;
}


void CncActuatorDcMotor::Test_PwmSpeed(bool dir_is_cw,  uint32_t pwm_speed){
    this->__h_bridge->SetPwmSpeed(dir_is_cw, pwm_speed);
}






