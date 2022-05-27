#pragma once
#include "single_wheel_driver_base.h"
#include <stdint.h>

class DualWheelsDualSerialDriver: public SingleWheelDriverBase{
    public:
        void SetVelocity_in_percent(int8_t velocity) override;


    private:
};
