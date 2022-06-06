#pragma once
#include <stdint.h>
#include <HardwareSerial.h>

class JettySensorBase{
    public:
        JettySensorBase(){};
        virtual int16_t ReadAlignmentError(){};
        virtual void Update(){};
        float UpperLeft_distance_in_mm(){return this->__upper_left_distance_in_mm;};
        float UpperRight_distance_in_mm(){return this->__upper_right_distance_in_mm;};
        float LowerLeft_distance_in_mm(){return this->__lower_left_distance_in_mm;};
        float LowerRight_distance_in_mm(){return this->__lower_right_distance_in_mm;};

    private:
        float __upper_left_distance_in_mm;
        float __upper_right_distance_in_mm;
        float __lower_left_distance_in_mm;
        float __lower_right_distance_in_mm;

};

