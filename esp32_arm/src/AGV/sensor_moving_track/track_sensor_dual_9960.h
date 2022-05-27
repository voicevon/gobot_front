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
#include "AGV/sensor_moving_track/track_sensor_base.h"
#include "smarter_9960.h"
#include "AGV/light/light_base.h"


class TrackSensor_Dual9960_Config{
    public:
        uint8_t pin_left_sensor_sda = 26;
        uint8_t pin_left_sensor_sclk = 25;
        uint8_t pin_right_sensor_sda = 33;
        uint8_t pin_right_sensor_sclk = 32;
        uint8_t pin_WS2812_LED = 5;
        uint8_t LedWs2812B_counts = 10; // Popular NeoPixel ring size
};

class TrackSensor_Dual9960:public TrackSensorBase{
    public:
        TrackSensor_Dual9960();
        void Init(Adafruit_APDS9960* left_apds9960, Adafruit_APDS9960* right_apds9960);
        int16_t ReadAlignmentError() override;
        void ReadParkingError(int16_t* x_error, int16_t* y_error) override;
        void ClearFlag_Slowdown() override;
        void ClearFlag_SpeedUp();
        bool GetFlag_Slowdown() override;
        bool GetFlag_Speedup()override;
        void SayHello();
        
    protected:

    private:
        TrackSensor_Dual9960_Config* __config;
        Smarter9960* left_sensor;
        Smarter9960* right_sensor;
        /*
        0:  begin with setting flag to 0.
        1:  sensor set flag to 1 , when sensor detected mark.
        2:  apps set to +1, when the app take some action.
        3:  sensor set to +1, when sensor detected new mark, and flag == 1.
        0:  sensor set to 0, when flag >= 3
        */
        uint8_t __flag_spped_up;
        uint8_t __flag_slow_down;
        // Adafruit_NeoPixel* pixels;
};