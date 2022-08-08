#include "actuator_dc_motor.h"
#include "Arduino.h"
#include "MyBoards/board_base.h"

#define PWM_RESOLUTION 8
#define PWM_FREQUENCY 20
#define GEAR_TEETH_COUNT 56.0f
#define GEAR_PITCH 12.7f   //unit is mm
#define PID_P 1.0f
#define INERTIA_DISTANCE_IN_MM  50   // ??  in_rad


void ActuatorDcMotor::SpinOnce(){
    // real speed control, position check, auto stop....

    if(this->GetAbsDistanceToTarget_InCncUnit() * GEAR_PITCH < INERTIA_DISTANCE_IN_MM){
        // The wheel will continue to run a short time after stoping, because the inertia.
        // TDDO:  How to deal with negtive distance?
        this->Stop();
    }else{
        // control speed
        float error = this->__sensor->getVelocity() - this->__cnc_speed; 
        float new_speed = - PID_P * error;   //   pid.get_speed(error);
        this->UpdateSpeedWhenMotorIsRunning(new_speed);
    }
}

float ActuatorDcMotor::GetCurrentPosition_InCncUnit(){
    // from sensor_angle to cnc_angle.
    // cnc_angle == sensor_angle  * (10 / 10) * (10 / 56) * (56 / 157) 
    //           == sensor_angle * (SENSOR_GEAR_COUNT / MOTOR_GEAR_COUNT)* (MOTOR_GEAR_COUNT / DRIVER_GEAR_COUNT) * (DRIVER_GEAR_COUNT / CHAIN_PITCH_COUNT)
    //           == sensor_angle * (SENSOR_GEAR_COUNT / CHAIN_PITCH_COUNT)

    float cnc_position = (this->__sensor->getAngle() + this->__sensor_offset) * this->__SLOPE_FROM_SENSOR_TO_CND;
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

void ActuatorDcMotor::UpdateSpeedWhenMotorIsRunning(float new_cnc_speed){
        this->__pwm_speed = 12.23f * new_cnc_speed;   // todo:   ax^3 + bx^2 + cx + d
        bool dir_is_cw = (this->_target_cnc_position - this->GetCurrentPosition_InCncUnit()) > 0;
        this->SetPwmSpeed(dir_is_cw, this->__pwm_speed);
}

void ActuatorDcMotor::SetSpeed(float rad_per_second){
    this->__cnc_speed = rad_per_second;
}

void ActuatorDcMotor::SetAccelleration(float accelleration_in_cnc_unit){
    // this is a future feature.
}

void ActuatorDcMotor::Stop(){
    // motor A and motor B setted LOW to stop DC motor
    digitalWrite(this->__h_bridge_pin_dir, LOW);
    ledcWrite(this->__pwm_channel, 0);
}

void ActuatorDcMotor::SetPwmSpeed(bool dir_is_cw,  uint32_t pwm_speed){
    if(dir_is_cw){
        // Serial.println (pwm_speed);
        // this->PrintOut();
        digitalWrite(this->__h_bridge_pin_dir, LOW);
        ledcWrite(this->__pwm_channel, pwm_speed);
    }
    else {
        // make  DCmotor CCW 
        ledcWrite(this->__pwm_channel, (256 - pwm_speed));
        digitalWrite(this->__h_bridge_pin_dir, HIGH);
    }
}



ActuatorDcMotor::ActuatorDcMotor(uint8_t h_bridge_pin_dir, uint8_t h_bridge_pin_speed){
    pinMode(h_bridge_pin_dir, OUTPUT);
    pinMode(h_bridge_pin_speed, OUTPUT);
    digitalWrite(h_bridge_pin_dir, LOW);

    // init ledc via assign ledc channel
    BoardBase board;

    this->__pwm_channel = board.Assign_ledc_channel();
	ledcAttachPin (h_bridge_pin_speed, this->__pwm_channel);
	ledcSetup (this->__pwm_channel, PWM_FREQUENCY, PWM_RESOLUTION);
    ledcWrite(this->__pwm_channel, 0);

    this->__h_bridge_pin_dir = h_bridge_pin_dir;
    this->__h_bridge_pin_speed = h_bridge_pin_speed;

    // Calculate rad_per_mm,  This is determined by mechanic designer.
    // this->__sensor_rad_per_mm = GEAR_PITCH * GEAR_TEETH_COUNT / TWO_PI; 
    this->__sensor_offset = 0;
    this->__SLOPE_FROM_SENSOR_TO_CND = float(this->SENSOR_GEAR_COUNT) / this->CHAIN_PITCH_COUNT;
    this->__SLOPE_FROM_CNC_TO_SENSOR = float(this->CHAIN_PITCH_COUNT) / this->SENSOR_GEAR_COUNT;
}

void ActuatorDcMotor::PrintOut(){

    Logger::Debug("ActuatorDcMotor::ActuatorDcMotor()");
    Logger::Print("this->__pwm_channel ",this->__pwm_channel );
    Logger::Print("this->__h_bridge_pin_dir ",this->__h_bridge_pin_dir );
    Logger::Print("this->__h_bridge_pin_speed ",this->__h_bridge_pin_speed );
    Serial.println(FCBC_RESET);
}
