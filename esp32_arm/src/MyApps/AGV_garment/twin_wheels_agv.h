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
.   Fast Moving  --> Slow_Moving ------------------>   Parking  ---------> Parked ---^  
.    [slow mark]                       [Park command]   (Super slow)[Sleep command] 
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

    protected:
        AGV_STATE _State;
        void SetForwardSpeed(float target_speed){};
        void MovingLoop(int track_error){};
        void SetWheelSpeed(float left_speed, float right_speed){};

    private:
        float __fast_velocity = 100;
        float __slow_velocity = 60;
        float __parking_velocity = 10;

        TrackSensor_Dual9960* trackSensor;
        UltraSonicDistanceSensor *obstacleSensor; // = UltraSonicDistanceSensor(HS04_PIN_ECHO,HS04_PIN_TRIG); //initialisation class HCSR04 (trig pin , echo pin)

        float common_speed;
        float diff_speed;  //left faster is positive.
        bool DoParking();

};