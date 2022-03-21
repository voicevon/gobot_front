#pragma once

/*
.             TRACK                                    TRACK
.(WHITE)     (BLACK)      (WHITE)
.         |**********|                              |green green| 
.         |**********|                              |green green| 
.         |**********|                              |green green| 
.         |**********|                              |green green| 
.         |**********|                              |red    red | 
.         |**********|                              |red    red | 
.         |**********|                              |red    red | 
.         |**********|                          |-- |red    red |--|
.     |-------|  |-------|                      |   |black black|  |    
.     |       |  |       |                      |       |  |       |
.     |  left |  | right |                      |       |  |       |
.     | sensor|  | sensor|                      |       |  |       |
.     | window|  | window|                      |-------|  |-------|
.     |-------|  |-------|  
.
.
.
.
.             Forward to.  Y   
.                          ^                  
.                          |                  
.                          |                  
.                          |                  
.                          |                  
.                          |                  
.                   --------------> X
.
.
*/

#include "stdint.h"
#include "Wire.h"
#include "Adafruit_APDS9960.h"

class TrackSensor_Dual9960{
    public:
        TrackSensor_Dual9960(uint8_t left_sensor_pin_sda, uint8_t left_sensor_pin_sclk, uint8_t right_sensor_pin_sda, uint8_t right_sensor_pin_sclk);
        int16_t SpinOnce_Forwarding();
        void SpinOnce_Parking(int16_t* x_error, int16_t* y_error);
        // float ReadError_X();
        // float ReadError_Y();
        void ClearFlag_Slowdown();
        void ClearFlag_SpeedUp();
        bool GetFlag_Slowdown(){if (this->__flag_slow_down == 1) return true; return false;};
        bool GetFlag_Speedup(){if (this->__flag_spped_up == 1) return true; return false;};;

    private:
        Adafruit_APDS9960* __apds_left;
        Adafruit_APDS9960* __apds_right;
        Adafruit_APDS9960* __current_sensor;
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