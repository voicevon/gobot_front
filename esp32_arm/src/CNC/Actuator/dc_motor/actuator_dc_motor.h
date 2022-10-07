#pragma once
#include "../cnc_actuator_base.h"
#include "Robot/driver/h_bridge/h_bridge.h"
#include <SimpleFOC.h>  //For encoder only


class ActuatorDcMotor: public CncActuatorBase{
    public:
        ActuatorDcMotor(){};

        // void LinkAngleSensor(RotaryEncoder* sensor){this->__sensor=sensor;}; 
        void LinkEncoder(Encoder* encoder){this->__encoder=encoder;};
        void LinkPidController(PIDController* pid){this->__speed_pid=pid;};
        void LinkHBridgeDriver(H_Bridge* h_bridge){this->__h_bridge=h_bridge;};  
        void SpinOnce() override;

        // Will auto change to false, when arrived(very closed to) target position during moving.
        // Also mightly auto change to true, After SetTargetPositionTo() is invoked.
        void ForceStop();   //Only G28 is using this.
        void UpdateMovement(MoveBlock_SingleActuator* movement) override;


        //===================================================================
        // For Being an actuator

        // float GetCurrentPosition() override;
        // void InitFormular_FromCncPosition(float position_in_cnc_unit) override;

        // float GetAbsDistanceToTarget_InCncUnit() override;
        // When motor is running, should not effect running speed. 
        // Will take effection when invoke SpinOnce().
        // void UpdateTargetPositionFromCurrent() override;
        // float GetSpeed() override {return abs(this->__target_velocity);};  //todo:  read speed from sensor, rename to GetVelocity

        void Test_PwmSpeed(bool dir_is_cw,  uint32_t pwm_speed);

    protected:
        
    private:
        // My components
        H_Bridge* __h_bridge;
        Encoder* __encoder;
        // RotaryEncoder* __sensor;

        // speed control
        PIDController* __speed_pid;
        float __target_velocity;   // cnc rad per second.

        // int __count_down_for_serial_print = 0;
        LowPassFilter __filter = LowPassFilter(0.05f);

        void __SpinOnce_FollowVelocity(float velocity);

};