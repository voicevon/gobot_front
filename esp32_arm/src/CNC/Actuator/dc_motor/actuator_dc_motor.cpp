#include "actuator_dc_motor.h"
#include "Arduino.h"
#include "MyBoards/board_base.h"


#define GEAR_TEETH_COUNT 56.0f
#define GEAR_PITCH 12.7f   //unit is mm
#define PID_P 10.0f
#define INERTIA_DISTANCE_IN_MM  0.0001   // ??  in_rad



void ActuatorDcMotor::SpinOnce(){
    this->__sensor->update();
    // real speed control, position check, auto stop....
    float abs_distance = this->GetAbsDistanceToTarget_InCncUnit();
    // if(this->GetAbsDistanceToTarget_InCncUnit() * GEAR_PITCH < INERTIA_DISTANCE_IN_MM){
    if (false){
        // The wheel will continue to run a short time after stoping, because the inertia.
        // TDDO:  How to deal with negtive distance?
        this->Stop();
    }else{
        // control speed
        this->__count_down--;
        if (this->__count_down <=0){
            Logger::Print("Abs DIstanceToTarget in CNC Rad ", abs_distance);
            this->__count_down = 18888;
        }

        // float velocity =  this->__filter(this->__sensor->getVelocity());
        // this->__speed_pid->P = 1;
        float velocity =  this->__sensor->getVelocity();
        float speed_error =  velocity - this->__cnc_speed / 30;
        float pwm_speed =  - this->__speed_pid->FeedError(speed_error);
        // float pwm_speed = - PID_P * error;   //   pid.get_speed(error);

 
        // Serial.print("velocity of sensor, speed_error, pwm_speed \t");
        Serial.print(velocity);
        Serial.print("\t");
        Serial.print(speed_error);
        Serial.print("\t");
        Serial.print(pwm_speed);
        Serial.println();
        // todo:   ax^3 + bx^2 + cx + f
        bool dir_is_cw = (this->_target_cnc_position - this->GetCurrentPosition_InCncUnit()) > 0;
        dir_is_cw = true;
        // pwm_speed = constrain(pwm_speed, 0 , 255);
        if (pwm_speed > 255.0f) pwm_speed = 255.0f;
        if (pwm_speed < 1.0f) pwm_speed = 0.0f;
        this->SetPwmSpeed(dir_is_cw, pwm_speed);
    }
}

float ActuatorDcMotor::GetCurrentPosition_InCncUnit(){
    // from sensor_angle to cnc_angle.
    // cnc_angle == sensor_angle  * (10 / 10) * (10 / 56) * (56 / 157) 
    //           == sensor_angle * (SENSOR_GEAR_COUNT / MOTOR_GEAR_COUNT)* (MOTOR_GEAR_COUNT / DRIVER_GEAR_COUNT) * (DRIVER_GEAR_COUNT / CHAIN_PITCH_COUNT)
    //           == sensor_angle * (SENSOR_GEAR_COUNT / CHAIN_PITCH_COUNT)

    float cnc_position = (this->__sensor->getAngle() + this->__sensor_offset) * this->__SLOPE_FROM_SENSOR_TO_CNC;
    return cnc_position;
}

void ActuatorDcMotor::SetCurrentPositionAs(float position_in_cnc_unit){
    this->__sensor_offset = (position_in_cnc_unit * this->__SLOPE_FROM_CNC_TO_SENSOR - this->__sensor->getAngle());
}

void ActuatorDcMotor::SetTargetPositionTo(bool is_absolute_position, float position_in_cnc_unit){
    Logger::Debug("ActuatorDcMotor::SetTargetPositionTo()  is entering");
    Logger::Print("is_absolute_position", is_absolute_position);
    Logger::Print("position_in_degree", TWO_PI * position_in_cnc_unit);
    
    if (is_absolute_position){
        this->_target_cnc_position = position_in_cnc_unit;
    }else{
        this->_target_cnc_position = this->GetCurrentPosition_InCncUnit() + position_in_cnc_unit;
    }
}

float ActuatorDcMotor::GetAbsDistanceToTarget_InCncUnit(){
    // sensor --> current poistion   --> distance to target
    // TODO:  minus distance.
    return this->_target_cnc_position - this->GetCurrentPosition_InCncUnit();
}



void ActuatorDcMotor::SetSpeed(float rad_per_second){
    this->__cnc_speed = rad_per_second;
}

void ActuatorDcMotor::SetAccelleration(float accelleration_in_cnc_unit){
    // this is a future feature.
}

void ActuatorDcMotor::Stop(){
    this->__h_bridge->Stop();
}
//     // motor A and motor B setted LOW to stop DC motor
//     digitalWrite(this->__h_bridge_pin_dir, LOW);
//     ledcWrite(this->__pwm_channel, 0);
// }

void ActuatorDcMotor::SetPwmSpeed(bool dir_is_cw,  uint32_t pwm_speed){
    this->__h_bridge->SetPwmSpeed(dir_is_cw, pwm_speed);
}
//     if(dir_is_cw){
//         // Serial.println (pwm_speed);
//         // this->PrintOut();
//         digitalWrite(this->__h_bridge_pin_dir, LOW);
//         ledcWrite(this->__pwm_channel, pwm_speed);
//     }
//     else {
//         // make  DCmotor CCW 
//         ledcWrite(this->__pwm_channel, (256 - pwm_speed));
//         digitalWrite(this->__h_bridge_pin_dir, HIGH);
//     }
// }




ActuatorDcMotor::ActuatorDcMotor(uint8_t h_bridge_pin_dir, uint8_t h_bridge_pin_speed){


    // Calculate rad_per_mm,  This is determined by mechanic designer.
    // this->__sensor_rad_per_mm = GEAR_PITCH * GEAR_TEETH_COUNT / TWO_PI; 
    this->__sensor_offset = 0;
    this->__SLOPE_FROM_SENSOR_TO_CNC = float(this->SENSOR_GEAR_COUNT) / this->CHAIN_PITCH_COUNT;
    this->__SLOPE_FROM_CNC_TO_SENSOR = float(this->CHAIN_PITCH_COUNT) / this->SENSOR_GEAR_COUNT;
}

void ActuatorDcMotor::PrintOut(){

    Logger::Debug("ActuatorDcMotor::ActuatorDcMotor()");
    // Logger::Print("this->__pwm_channel ",this->__pwm_channel );
    // Logger::Print("this->__h_bridge_pin_dir ",this->__h_bridge_pin_dir );
    // Logger::Print("this->__h_bridge_pin_speed ",this->__h_bridge_pin_speed );
    Serial.println(FCBC_RESET);
}
