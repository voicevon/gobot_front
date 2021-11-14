#pragma once

#include "SimpleFOC/common/pid.h"

enum AGV_DIRECTION{
    FORWARD = 1,
    BACKWARD = 2,
};

class AGV{
    public:
        AGV();
        void Init(PIDController* wheel_pid);
        void Move(AGV_DIRECTION direction, int speed_in_percent);
        int ReadRailSensor();
        void SpinOnce();
        float common_speed = 0.0f;

    private:
        PIDController* wheel_pid;
        float left_wheel_speed = 0.0f;
        float right_wheel_speed = 0.0f;


};