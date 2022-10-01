# pragma once
#include <stdint.h>
#include <HardwareSerial.h>
class AgvMoverBase{
    public:
        // virtual void SetSingleSpeed(int speed_in_percent);
        virtual void SetForwdingSpeed(int forwarding_speed_in_percent, float angle);
};