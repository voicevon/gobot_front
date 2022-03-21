#pragma once
#include "all_devices.h"
#ifdef I_AM_GARMENT_BOT


#include "twin_wheels/vehical_twin_wheel_hw.h"
#include "twin_wheels/vehical_twin_wheel.h"
#include "box_mover/box_mover.h"
#include "AGV/map_navigator.h"
#include "remote_sensor.h"
#include "garment_agv.h"

// #define PIN_LED_POWER_2130 23






class GarmentBot{
    public:

        GarmentBot();
        BoxMover objBoxMover = BoxMover();
        VehicalTwinWheel objTwinWheel = VehicalTwinWheel();
        TwinWheelHardware objTwinWheelHardware = TwinWheelHardware();

        void Init();
        void SpinOnce();
        void ToState(GarmentAgv::GARMENTAGV_STATE state);
        void Test(int test_id);
        uint8_t GetMqtt_PubPayload(uint8_t* chars);
        void onMqttReceived(uint8_t* payload);
        
    protected:
        GarmentAgv::GARMENTAGV_STATE _State;
        GarmentAgv::GARMENTAGV_STATE _last_state;
        void SpinOnce_Working();
        
    private:
        RemoteSensor objRemoteSensor;
        TrackGraph objMapNavigator;
        
        void onDetectedMark(uint16_t mapsite_id);
        BranchNode __current_BranchNode;
        uint16_t _ID = 0;

};



#endif