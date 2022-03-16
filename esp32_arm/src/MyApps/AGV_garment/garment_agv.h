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
.      <--------------------------------------------------------------------------------------------\
,     |                                                                                              | 
,    F_Paused          S_Paused                       P_Paused                                       |
.     |  ^             |     ^                         |    ^                                        ^                             
.     |  |             |     |                         |    |                                        |                           
.   Fast Moving  --> Slow_Moving ----------------->   Parking  --->   Parked --> Robot_Loading  -----^  
.                    (Read Mark RFID)                 (Super slow)       |                           |
.                                                                        |-------> Robot_Unloading --^
.                                                                        |                           |
.                                                                        |-------> Charging ---------^
.                                                                        |                           |
.                                                                         --------> Sleeping --------^
.                                                                        |                           |
.                                                                         --------> Charging --------^
*/


#pragma once
#include "AGV/map_navigator.h"
#include "twin_wheels/vehical_twin_wheel_hw.h"
#include "track_sensor_dual_9960.h"
#include "HCSR04.h"            //https://github.com/gamegine/HCSR04-ultrasonic-sensor-lib
// #include "Adafruit_APDS9960.h"  //https://github.com/adafruit/Adafruit_APDS9960/blob/master/examples/color_sensor/color_sensor.ino
#include <SPI.h>
#include "MFRC522.h"            //https://github.com/miguelbalboa/rfid/blob/master/examples/DumpInfo/DumpInfo.ino
// #define RST_PIN 9       // Configurable, see typical pin layout above
// #define SS_PIN 10      // Configurable, see typical pin layout above

#define HS04_PIN_ECHO 2
#define HS04_PIN_TRIG 3

class GarmentAgv: public TwinWheelHardware{
    
    public:
        enum GARMENTAGV_STATE{
            SLEEPING = 0,
            FAST_MOVING,
            FAST_MOVING_PAUSED,
            SLOW_MOVING,
            SLOW_MOVING_PAUSED,
            PARKING,
            PARKING_PAUSED,
            PARKED,
            CHARGING,
            LOADING,
            UNLOADING,
            // EMERGENCY_STOPING,
        };
        GarmentAgv();
        // Adafruit_APDS9960 apds;


        UltraSonicDistanceSensor objHS04 = UltraSonicDistanceSensor(HS04_PIN_ECHO,HS04_PIN_TRIG); //initialisation class HCSR04 (trig pin , echo pin)
        MFRC522 objRfidReader = MFRC522(10, 9);  // Create MFRC522 instance
        void SpinOnce();
        void ToState(GarmentAgv::GARMENTAGV_STATE state);
        bool found_obstacle = true;

    protected:
        protected:
        GARMENTAGV_STATE _State;
        GARMENTAGV_STATE _last_state;
        void SpinOnce_Working();
        MapNavigator objMapNavigator;

    private:
        MapSite __current_mapsite;

};