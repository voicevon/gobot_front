#pragma once
#include "twin_wheels_base.h"
#include "single_wheel_bldc.h"
class TwinWheels_BLDC: public TwinWheelsBase{
    public:
        TwinWheels_BLDC(){};
        SingleWheel_BLDC objLeftWheel = SingleWheel_BLDC();
        SingleWheel_BLDC objRightWheel;
};
