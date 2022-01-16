#pragma once
#include "all_devices.h"
#ifdef I_AM_GARMENT_BOT


#include "agv_slim_hw.h"
#include "box_mover.h"

// #define PIN_LED_POWER_2130 23


/*
.     |<-----------^   <------------   <---------------------------
.     |            ^  |            ^  |                           ^
.   Sleeping  --> Agv_Moving  -->  Parked --> Robot_Loading  -----^  
.                                   |                             ^
.                                   |---------> Robot_Unloading --^
.                                   |                             ^
.                                   |---------> Charging ---------^
*/

class GarmentBot{
    public:
        enum GARMENTBOT_MODE{
            SLEEP = 0,
            MOVING = 2,
            EMERGENCY_STOPING,
            CHARGING,
            MOVING_PAUSED,
            LOADING,
            UNLOADING,
        };
        GarmentBot(){};

        void Init();
        void SpinOnce();
        void SetMode(GARMENTBOT_MODE mode);
        void ExecuteCommand(int topic, int payload);
        void Test(int test_id);

    protected:
        GARMENTBOT_MODE _State;
        void SpinOnce_Working();
        
    private:
        AgvSlimHardware agv = AgvSlimHardware();
        BoxMover boxMover = BoxMover();
        TrackSensor_Shengteng objTrackSensor = TrackSensor_Shengteng();
        int objObstacleDetector;

};



#endif