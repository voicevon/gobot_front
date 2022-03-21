#pragma once

/*
.             TRACK                                    TRACK
.(WHITE)     (BLACK)      (WHITE)
.         |**********|                              |red    red | 
.         |**********|                              |red    red | 
.         |**********|                          |-- |red    red |--|
.         |**********|                          |   |green green|  |
.         |**********|                          |   |green green|  |
.         |**********|                          |   |green green|  | 
.     |-------|  |-------|                      |   |black black|  |    
.     |       |  |       |                      |       |  |       |
.     |  left |  | right |                      |-------|  |-------|
.     | sensor|  | sensor|  
.     | window|  | window|  
.     |-------|  |-------|  

*/

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
        void ClearFlag_Slowdown();
        void ClearFlag_SpeedUp();
        bool GetFlag_Slowdown(){if (this->__flag_slow_down == 1) return true; return false;};
        bool GetFlag_Speedup(){if (this->__flag_spped_up == 1) return true; return false;};;
        FOLKING __folking;

    private:
        // TwoWire* __i2c_bus_at_left;
        // TwoWire* __i2c_bus_at_right;
        Adafruit_APDS9960* __apds_left;
        Adafruit_APDS9960* __apds_right;

        /*
        0:  begin with setting flag to 0.
        1:  sensor set flag to 1 , when sensor detected mark.
        2:  apps set to +1, when the app take some action.
        3:  sensor set to +1, when sensor detected new mark, and flag == 1.
        0:  sensor set to 0, when flag >= 3
        */
        uint8_t __flag_spped_up;
        uint8_t __flag_slow_down;


        
        

};