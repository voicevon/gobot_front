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
#include "AGV/agv_sensor_base.h"
#include "track_sensor_smart_9960.h"
#include <Adafruit_NeoPixel.h>


class TrackSensor_Dual9960_Config{
    public:
        uint8_t pin_left_sensor_sda = 21;
        uint8_t pin_left_sensor_sclk = 22;
        uint8_t pin_right_sensor_sda = 23;
        uint8_t pin_right_sensor_sclk = 15;
        uint8_t pin_led = 19;
        uint8_t LedWs2812B_counts = 10; // Popular NeoPixel ring size
};

class TrackSensor_Dual9960: public AgvTrackSensor{
    public:
        TrackSensor_Dual9960(TrackSensor_Dual9960_Config* config);
        int16_t ReadForwardingError();
        void ReadParkingError(int16_t* x_error, int16_t* y_error);
        void ClearFlag_Slowdown();
        void ClearFlag_SpeedUp();
        bool GetFlag_Slowdown(){if (this->__flag_slow_down == 1) return true; return false;};
        bool GetFlag_Speedup(){if (this->__flag_spped_up == 1) return true; return false;};
        bool IsFollowingLeft;
        void TurnOnLed(bool turn_on);
        void SayHello();

    private:
        TrackSensor_Dual9960_Config* __config;
        SmartLightSensor* left_sensor;
        SmartLightSensor* right_sensor;
        /*
        0:  begin with setting flag to 0.
        1:  sensor set flag to 1 , when sensor detected mark.
        2:  apps set to +1, when the app take some action.
        3:  sensor set to +1, when sensor detected new mark, and flag == 1.
        0:  sensor set to 0, when flag >= 3
        */
        uint8_t __flag_spped_up;
        uint8_t __flag_slow_down;
        Adafruit_NeoPixel* pixels;
};