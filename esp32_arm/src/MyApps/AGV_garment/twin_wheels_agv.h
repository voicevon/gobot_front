/*
.   Objects:
.       BoxMover
.       TwinWheels
.
.   Sensors:
.       I2C-A:  APDS9960  left
.       i2C-B:  APDS9960 right
.       SPI:    RFID Reader
-       ADC:    Battery
.       ADC:    Charger     
.       GPIO:   HS04
*/




/*    
.     
.     
.      <-----------------------------------------------------------------------------\
,     |                               ^                                              | 
,    F_Paused          S_Paused       |                P_Paused                      |
.     |  ^             |     ^        |[Fast mark]      |    ^                       | [Command Move]                      ^                             
.     |  |             |     |        |                 |    |                       |                           
.   Fast Moving  --> Slow_Moving ------------------>   Parking  --->   Parked -------^  
.    [slow mark]                       [Park command]   (Super slow) 
.   from track sensor                                                    
.                                                                        
.                                                                        
.                                                                        
.                                                                                                                              |                           |
.                                                                        
*/


#pragma once
#include "AGV/map_navigator.h"
// #include "twin_wheels/vehical_twin_wheel_hw.h"
// #include "twin_wheels/twin_wheels_base.h"
#include "twin_wheels/twin_wheels_bldc.h"
// #include "AGV/agv_sensor_base.h"
#include "HCSR04.h"            //https://github.com/gamegine/HCSR04-ultrasonic-sensor-lib
// #include "Adafruit_APDS9960.h"  //https://github.com/adafruit/Adafruit_APDS9960/blob/master/examples/color_sensor/color_sensor.ino
#include <SPI.h>
// #define RST_PIN 9       // Configurable, see typical pin layout above
// #define SS_PIN 10      // Configurable, see typical pin layout above
#include "track_sensor_dual_9960.h"
#define HS04_PIN_ECHO 2
#define HS04_PIN_TRIG 3



class TwinWheelsAgv: public TwinWheels_BLDC{
    
    public:
        enum AGV_STATE{
            FAST_MOVING,
            FAST_MOVING_PAUSED,
            SLOW_MOVING,
            SLOW_MOVING_PAUSED,
            PARKING,
            PARKING_PAUSED,
            PARKED,
        };
        TwinWheelsAgv();
        void SpinOnce();
        void ToState(TwinWheelsAgv::AGV_STATE state);
        bool found_obstacle = true;
        TwinWheelsAgv::AGV_STATE GetState(){return this->_State;};
    protected:
        AGV_STATE _State;
        AGV_STATE _last_state;
        void SpinOnce_Working();
        // TrackGraph objTrackGraph;

    private:
        TrackSensor_Dual9960* trackSensor;
        HallSensor* sensor_left_wheel;

        BranchNode __current_navigator_point;
        bool DoParking();

};