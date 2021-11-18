#pragma once
/*
    This AGV is driven by 2 wheels. 
        each wheel is driven by L298N
    Track sensor is 8 bit in front of veheical frame.
*/
#include "agv_base.h"

class AgvGarment: public AgvBase{

    public:
        void Init(PIDController* wheel_pid, ActuatorBase* left_wheel, ActuatorBase* right_wheel);

        ActuatorBase* leftWheel;
        ActuatorBase* rightWheel;

    protected:
        void _SpinOnce_Enter() override;

    private:



};


