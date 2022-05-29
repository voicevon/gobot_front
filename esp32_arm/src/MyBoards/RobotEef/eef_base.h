#pragma once

#include <HardwareSerial.h>
#include <stdint.h>

class RobotEefBase{
    public:
        virtual void Run(uint8_t eef_code);

    private:
    
};



