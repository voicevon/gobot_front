#pragma once

#include "libs/capacitive_sensor/capacitive_sensor.h"
// https://www.pjrc.com/teensy/td_li bs_CapacitiveSensor.html

class TouchpadChannel{
    public:
        void Init(uint8_t pin_sensor);
        bool Is_Died(){return __is_died;};
        void Read();
        
        // 254: channel is died
        // 255: chinnel is not installed.
        // minimum value = 1;
        uint8_t GetValue(){return __byte_value;};

    private:
        bool __is_died;
        CapacitiveSensor* __sensor;
        // 254: channel is died
        // 255: chinnel is not installed.
        // minimum value = 1;
        uint8_t __byte_value;
        

};