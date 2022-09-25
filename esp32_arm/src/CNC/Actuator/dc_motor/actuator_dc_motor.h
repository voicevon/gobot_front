#pragma once
#include "../actuator_base.h"
#include "CNC/mover/driver/h_bridge/h_bridge.h"
#include "CNC/Sensor/position_sensor/polor_encoder.h"



class ActuatorDcMotor: public ActuatorBase{
    public:

        //===================================================================
        // For being myself.

        ActuatorDcMotor();
        void PrintOut();
        void LinkAngleSensor(PolorEncoder* sensor){this->__sensor=sensor;}; 
        void LinkPidController(PIDController* pid){this->__speed_pid=pid;};
        void LinkMotorDriver(H_Bridge* h_bridge){this->__h_bridge=h_bridge;};  
        void SpinOnce();
        void SetPwmSpeed(bool dir_is_cw,  uint32_t pwm_speed);
        // Will auto change to false, when arrived(very closed to) target position during moving.
        // Also mightly auto change to true, After SetTargetPositionTo() is invoked.
        bool IsMoving(){return this->__is_moving;};


        //===================================================================
        // For Being an actuator

        float GetCurrentPosition() override;
        void SetCurrentPositionAs(float position_in_cnc_unit) override;
        void SetTargetPositionTo(bool is_absolute_position, float target_position) override;
        float GetAbsDistanceToTarget_InCncUnit() override;
        // When motor is running, should not effect running speed. 
        // Will take effection when invoke SpinOnce().
        void SetSpeed(float speed_in_cnc_unit) override;   
        void UpdateTargetPositionFromCurrent() override;
        float GetSpeed() override {return this->__target_speed;};
        void SetAccelleration(float accelleration_in_cnc_unit) override;

    protected:
        
    private:

        // My components
        H_Bridge* __h_bridge;
        PolorEncoder* __sensor;  //todo :  rename to encoder or amgle_sensor or position_sensor

        // speed control
        PIDController* __speed_pid;
        float __target_speed;   // cnc rad per second.
        bool __is_moving = false;

        int __count_down_for_serial_print = 0;
        LowPassFilter __filter = LowPassFilter(0.05f);



};