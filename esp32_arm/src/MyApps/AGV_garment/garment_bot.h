#pragma once
#include "all_devices.h"
#ifdef I_AM_GARMENT_BOT


#include "vehical_twin_wheel_hw.h"
#include "box_mover.h"
#include "AGV/map_navigator.h"

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
.                                                                         --------> Sleeping ---------^
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
        void ExecuteCommand(int topic, int payload);
        void Test(int test_id);

    protected:
        GARMENTBOT_STATE _State;
        GARMENTBOT_STATE _last_state;
        void SpinOnce_Working();
        void CommuWithUppper();
    private:
        TwinWheelHardware objTwinWheel = TwinWheelHardware();
        BoxMover boxMover = BoxMover();
        TrackSensor_Shengteng objTrackSensor = TrackSensor_Shengteng();
        TrackSensor_Shengteng objPositionSensor = TrackSensor_Shengteng();
        int objObstacleDetector;
        uint16_t objRemoteRfidReader;
        MapNavigator objMapNavigator;
        


};



#endif