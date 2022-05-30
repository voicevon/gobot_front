#pragma once
#define TWO_PI 6.283185307179586476925286766559
#include <HardwareSerial.h>
#include <stdint.h>


class CncConfigBase{
    public:
        virtual int32_t GetStepsPerUnit(char axis_name);
        
    private:


};