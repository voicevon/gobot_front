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
#include "HCSR04.h"            //https://github.com/gamegine/HCSR04-ultrasonic-sensor-lib
#include <SPI.h>
#include "track_sensor_dual_9960.h"
#define HS04_PIN_ECHO 2
#define HS04_PIN_TRIG 3

#include "SimpleFOC.h"

class TwinWheelsAgv{
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
        void SetWheelSpeed(float left_speed, float right_speed){};

        void Init(){};
        void LinkPid(PIDController* wheel_pid){};
        void MoveForward(int track_error){};
        void SetForwardSpeed(float target_speed){};

    protected:
        AGV_STATE _State;
        AGV_STATE _last_state;
        void SpinOnce_Working();

    private:
        TrackSensor_Dual9960* trackSensor;

        BranchNode __current_navigator_point;
        float common_speed;
        float diff_speed;  //left faster is positive.
        bool DoParking();

};