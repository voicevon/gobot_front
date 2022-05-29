#pragma once
#include "all_devices.h"
// #ifdef I_AM_ROBOT_ASRS_AGV

#include "IoT/mqtt_message_consumer.h"
#include "CNC/gcode/gcode_queue.h"
#include "CNC/cnc_core_yz/cnc_core_yz.h"
#include "ASRS/asrs_base.h"
#include "AGV/map_road_station/map_navigator.h"
#include "AGV/agv_base.h"
#include "MyBoards/asrs_agv_2205/board_asrs_agv.h"
#include "MyApps/bot_core_yz/box_carrier.h"


class MqttReportData{
    float battery_voltage;
    char bot_state;
};

// What I know:
//      1. mqtt command, and message queue of map and navigation. 
//      2. read RFID, determin vehical speed or parking.
//      3. Battery voltage.
//      4. Alignment sensors (2-channels IR sensor) 
//
// What I don't know:
//      1. track sensor, and position-Y error, PID controller 
//      4. Distance sensor, to detect obstacle.
//      2. Vehical speed, motor control.

class BotAsrsAgvCoreYZ: public MqttMessageConsumer{
    public:
        enum BOT_STATE{
            BOT_LOCATING = 0,
            BOT_SLEEPING = 1,
            AGV_MOVING_TO_SOURCE = 2,
            AGV_PARKED_AT_SOURCE = 3,
            ROBOT_LOAD_ALIGN = 4,
            ROBOT_LOADING = 5,
            AGV_MOVING_TO_DESTINATION = 6,
            AGV_PARKED_AT_DESTINATION = 7,
            ROBOT_UNLOAD_ALIGN = 8,
            ROBOT_UNLOADING = 9,
            BOT_CHARGING = 10,    //After AGV_MOVING_TO_SOURCE-->AGV_PARKED_AT_SOURCE
            BOT_EMERGENCY_STOPING = 11,
        };

        BotAsrsAgvCoreYZ(uint16_t id);
        void InitAllinOne(BoardAllInOne* board, StepControl* stepControl);

        AsrsBase objAsrs = AsrsBase();
        AgvBase objAgv = AgvBase();
        Cnc_CoreYZ cnc = Cnc_CoreYZ();

        BoxCarrier objBoxCarrier = BoxCarrier();  //??

        GcodeQueue* _gcode_queue;
        void SpinOnce() override;
        void ToState(BotAsrsAgvCoreYZ::BOT_STATE state);
        void Test(int test_id);
        uint8_t GetMqtt_PubPayload(uint8_t* chars);
        // void onMqttReceived(uint8_t* payload);
        
    protected:
        
    private:
        void ExecuteMqttCommand(const char* command) override;
        RoadGraph objMapNavigator;
        void onDetectedMark(uint16_t mapsite_id);
        RoadBranchNode __current_BranchNode;
        uint16_t _ID = 0;
        BotAsrsAgvCoreYZ::BOT_STATE __state;
        BoardAllInOne* board;
};



// #endif