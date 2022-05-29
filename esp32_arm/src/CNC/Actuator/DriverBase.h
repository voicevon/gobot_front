#pragma once

#include "Arduino.h"

class DriverBase{
    public:
        virtual void MoveAtSpeed(int pwm_speed, bool dir_forward);
        virtual void Stop();
        virtual void Break();
    
    private:

};