#pragma once

#include "libs/capacitive_sensor/capacitive_sensor.h"
// https://www.pjrc.com/teensy/td_li bs_CapacitiveSensor.html

class TouchpadChannel{
    public:
        void Init(uint8_t pin_sensor);
        bool Is_Died(){return __is_died;};
        void Read();

    private:
        bool __is_died;
        CapacitiveSensor* __sensor;
        // uint8_t __pin_sensor;
        

};