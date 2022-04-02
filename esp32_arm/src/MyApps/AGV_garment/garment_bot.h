#pragma once
#include "all_devices.h"
#ifdef I_AM_GARMENT_BOT


// #include "twin_wheels/vehical_twin_wheel_hw.h"
// #include "twin_wheels/vehical_twin_wheel.h"
#include "box_mover/box_mover.h"
#include "AGV/map_navigator.h"
#include "twin_Wheels/twin_wheels_agv.h"
#include <MFRC522.h>

// #define PIN_LED_POWER_2130 23


/*    
.     
.     
.                              <-------------------------------------------------------\
,                              |                                                       | 
.   Locating ------------>   Agv Moving ----> Agv Parked ---------> Robot_Loading  -----^  
.    (Slow)   [Got RFID]           (Read Mark RFID)           |
.                                 |-------> Robot_Unloading --^
.                                 |                           |
.                                 |-------> Charging ---------^
.                                 |                           |
.                                 |--------> Sleeping --------^
.                                 |                           |
.                                 |--------> Charging --------^
*/



// What I know:
//      1. mqtt command, and message queue of map and navigation. 
//      2. read RFID, determin vehical speed or parking.
//      3. Battery voltage.
// What I don't know:
//      1. track sensor, and position-Y error, PID controller 
//      4. Distance sensor, to detect obstacle.
//      2. Vehical speed, motor control.
class GarmentBot{
    public:
        enum BOT_STATE{
            BOT_LOCATING = 0,
            BOT_SLEEPING = 1,
            AGV_MOVING_TO_SOURCE = 2,
            AGV_PARKED_AT_SOURCE = 3,
            ROBOT_LOADING = 4,
            AGV_MOVING_TO_DESTINATION = 5,
            AGV_PARKED_AT_DESTINATION = 6,
            ROBOT_UNLOADING = 7,
            BOT_CHARGING = 8,
            BOT_EMERGENCY_STOPING = 9,
        };

        GarmentBot();
        BoxMover objBoxMover = BoxMover();
        TwinWheelsAgv objAgv = TwinWheelsAgv();

        void Init();
        void SpinOnce();
        void ToState(GarmentBot::BOT_STATE state);
        void Test(int test_id);
        uint8_t GetMqtt_PubPayload(uint8_t* chars);
        void onMqttReceived(uint8_t* payload);
        
    protected:
        
    private:
        TrackGraph objMapNavigator;
        MFRC522* __rfidReader;   // = MFRC522(10, 9);  // Create MFRC522 instance
        
        void onDetectedMark(uint16_t mapsite_id);
        BranchNode __current_BranchNode;
        uint16_t _ID = 0;
        GarmentBot::BOT_STATE __state;

};



#endif