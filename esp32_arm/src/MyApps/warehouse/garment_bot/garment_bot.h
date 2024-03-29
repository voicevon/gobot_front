#pragma once
#include "../select_app.h"
// #include "all_devices.h"
#ifdef I_AM_GARMENT_BOT

#include "AGV/agv_base.h"
#include "box_mover_agent.h"
#include "AGV/map_road_station/map_navigator.h"
// #include "AGV/sensor_map_site/smart_rfid_reader.h"
#define  PIN_BATTERY_VOLTAGE_ADC  34
#define  PIN_CHARGER_VOLTAGE_ADC  35




class MqttReportData{
    float battery_voltage;
    char bot_state;
};

// What I know:
//      1. mqtt command, and message queue of map and navigation. 
//      2. read RFID, determin vehical speed or parking.
//      3. Battery voltage.
// What I don't know:
//      1. track sensor, and position-Y error, PID controller 
//      4. Distance sensor, to detect obstacle.
//      2. Vehical speed, motor control.
class GarmentBot: public MqttMessageConsumer{
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

        GarmentBot(uint16_t id);
        GarmentBoxMoverAgent objBoxMoverAgent = GarmentBoxMoverAgent();
        AgvBase objAgv = AgvBase();

        void Init();
        void SpinOnce() override;
        void ToState(GarmentBot::BOT_STATE state);
        void Test(int test_id);
        uint8_t GetMqtt_PubPayload(uint8_t* chars);
        // void onMqttReceived(uint8_t* payload);
        
    protected:
        
    private:
        void onGot_MqttMessage(const char* command) override;
        RoadGraph objMapNavigator;
        SmartRfidReader objRfid;
        float __battery_voltage;
        void onDetectedMark(uint16_t mapsite_id);
        RoadBranchNode __current_BranchNode;
        uint16_t _ID = 0;
        GarmentBot::BOT_STATE __state;
};



#endif