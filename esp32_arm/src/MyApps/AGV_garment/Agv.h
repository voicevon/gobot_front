#pragma once

#include "SimpleFOC/common/pid.h"
#include "Robot/Actuator/DCMotor/h_bridge_l298n.h"


enum AGV_DIRECTION{
    FORWARD = 1,
    BACKWARD = 2,
};

class AGV{
    public:
        AGV();
        void Init(PIDController* wheel_pid, L298N* left_wheel, L298N* right_wheel);
        void Move(AGV_DIRECTION direction, int speed_in_percent);
        void Stop();
        int ReadRailSensor();
        void SpinOnce();
        float common_speed = 0.0f;
        L298N* leftWheel;
        L298N* rightWheel;

    private:
        PIDController* wheel_pid;
        float left_wheel_speed = 0.0f;
        float right_wheel_speed = 0.0f;


};