#pragma once

#include "SimpleFOC/common/pid.h"
#include "Robot/Actuator/ActuatorBase.h"
#include "Robot/Actuator/DCMotor/h_bridge_l298n.h"


enum AGV_DIRECTION{
    FORWARD = 1,
    BACKWARD = 2,
};

class AgvBase{
    public:
        AgvBase();
        // void Init(PIDController* wheel_pid, L298N* left_wheel, L298N* right_wheel);
        void Move(AGV_DIRECTION direction, int speed_in_percent);
        void Stop();
        virtual int ReadTrackSensor();
        void SpinOnce();
        float common_speed = 0.0f;
        // L298N* leftWheelDriver;
        // L298N* rightWheelDriver;
        PIDController* speed_pid;

    protected:
        virtual void _SpinOnce_Enter(){};
        virtual void _SpinOnce_Exit(){};
        int _track_pos_error;

    private:
        // float left_wheel_speed = 0.0f;
        // float right_wheel_speed = 0.0f;


};