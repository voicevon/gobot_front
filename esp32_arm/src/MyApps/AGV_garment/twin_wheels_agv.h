/*
.   Objects:
.       BoxMover
.       TwinWheels
.
.   Sensors:
.       I2C-A:  APDS9960  left
.       i2C-B:  APDS9960 right
.       GPIO:   HS04
*/


/*    
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
*/


#pragma once
#include "HCSR04.h"            //https://github.com/gamegine/HCSR04-ultrasonic-sensor-lib
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
        void Init(){};
        void SpinOnce();
        TwinWheelsAgv::AGV_STATE GetState(){return this->_State;};
        void ToState(TwinWheelsAgv::AGV_STATE state);

        void LinkPid(PIDController* wheel_pid){};
        void MoveForward(int track_error){};
        void SetForwardSpeed(float target_speed){};

    protected:
        AGV_STATE _State;
        void SetWheelSpeed(float left_speed, float right_speed){};

    private:
        TrackSensor_Dual9960* trackSensor;
        UltraSonicDistanceSensor *obstacleSensor; // = UltraSonicDistanceSensor(HS04_PIN_ECHO,HS04_PIN_TRIG); //initialisation class HCSR04 (trig pin , echo pin)

        float common_speed;
        float diff_speed;  //left faster is positive.
        bool DoParking();

};