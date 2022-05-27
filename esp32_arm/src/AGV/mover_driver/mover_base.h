# pragma once
#include <stdint.h>
#include <HardwareSerial.h>
class MoverBase{
    public:
        // virtual void SetSingleSpeed(int speed_in_percent);
        virtual void SetForwdingSpeed(uint8_t forwarding_speed_in_percent, float angle);
};