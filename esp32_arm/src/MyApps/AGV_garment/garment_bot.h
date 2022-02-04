#pragma once
#include "all_devices.h"
#ifdef I_AM_GARMENT_BOT


#include "twin_wheels/vehical_twin_wheel_hw.h"
#include "box_mover/box_mover.h"
#include "AGV/map_navigator.h"
#include "remote_sensor.h"

// #define PIN_LED_POWER_2130 23


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

class GarmentBot{
    public:
        enum GARMENTBOT_STATE{
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
        GarmentBot(){};

        void Init();
        void SpinOnce();
        void ToState(GARMENTBOT_STATE state);
        void Test(int test_id);
        uint8_t GetMqtt_PubPayload(uint8_t* chars);
        void onMqttReceived(uint8_t* payload);
    protected:
        GARMENTBOT_STATE _State;
        GARMENTBOT_STATE _last_state;
        void SpinOnce_Working();
    private:
        TwinWheelHardware objTwinWheel = TwinWheelHardware();
        BoxMover objBoxMover = BoxMover();
        RemoteSensor objRemoteSensor;
        MapNavigator objMapNavigator;
        
        void onDetectedMark(uint16_t mapsite_id);
        MapSite __current_mapsite;
        uint16_t _ID = 0;

};



#endif