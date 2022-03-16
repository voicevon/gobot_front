#pragma once



#include "stdint.h"
#include "Wire.h"
#include "Adafruit_APDS9960.h"

class TrackSensor_Dual9960{
    public:
        enum FOLKING{
            // left is white, right is black.
            FOLLOWING_LEFT = 1,
            // left is black, right is white.
            FOLLOWING_RIGHT = 2,
        };
        enum MODE{
            PARKING = 1,
            SLOW_MOVING = 2,
            FAST_MOVING = 3, 
            PARKED = 4,
        };

        TrackSensor_Dual9960(uint8_t left_sensor_pin_sda, uint8_t left_sensor_pin_sclk, uint8_t right_sensor_pin_sda, uint8_t right_sensor_pin_sclk);
        float ReadError_LeftRight();
        float ReadError_FrontRear();

    private:
        FOLKING __folking;
        // TwoWire* __i2c_bus_at_left;
        // TwoWire* __i2c_bus_at_right;
        Adafruit_APDS9960* __apds_left;
        Adafruit_APDS9960* __apds_right;


        
        

};