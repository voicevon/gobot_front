#pragma once
#include "SimpleFOC.h"

class TwinWheelsBase{
    public:
        void Init(){};
        void LinkPid(PIDController* wheel_pid){};
        void MoveForward(int track_error){};
        void SetForwardSpeed(float target_speed);
        void Stop(){};
        

        // BLDCDriver3PWM leftWheel = BLDCDriver3PWM(1,2,3,4);
        // BLDCDriver3PWM rightWheel = BLDCDriver3PWM(5,6,7,8);
    private:
        float common_speed;
        float diff_speed;  //left faster is positive.
};