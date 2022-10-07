#pragma once
#include "../cnc_actuator_base.h"
#include "Robot/driver/h_bridge/h_bridge.h"
#include <SimpleFOC.h>  //For encoder only


class CncActuatorDcMotor: public CncActuatorBase{
    public:
        CncActuatorDcMotor(){};

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
        void Test_PwmSpeed(bool dir_is_cw,  uint32_t pwm_speed);

    protected:
        
    private:
        // My components
        H_Bridge* __h_bridge;
        Encoder* __encoder;
        PIDController* __speed_pid;
        float __target_velocity;   // cnc rad per second.
        LowPassFilter __filter = LowPassFilter(0.05f);

        void __SpinOnce_FollowVelocity(float velocity);

};