#pragma once
#include "all_devices.h"
#ifdef I_AM_GARMENT_BOT_SINGLE_MCU

#include "IoT/mqtt_message_consumer.h"
#include "Robot/gcode_queue.h"

#include "MyApps/garment_box_carrier/box_carrier.h"
#include "MyApps/garment_box_carrier/box_carrier_hw.h"
#include "AGV/twin_Wheels/twin_wheels_agv.h"
#include "AGV/map_road_station/map_navigator.h"
#include "AGV/smart_rfid_reader.h"

#define  PIN_BATTERY_VOLTAGE_ADC  34
#define  PIN_CHARGER_VOLTAGE_ADC  35



/*    
.     
.     
.                              <---------------------------------------------------------\
,                              |                                                          | 
.   Locating ------------>   Agv Moving ----> Agv Parked ---------> Robot_Loading  -------^  
.    (Slow)   [Got RFID]        |  (Read Mark RFID)           |                           | 
.                               |                             |-------> Robot_Unloading --^
.                             (Fast)                          |                           |
.                             (Slow)                          |-------> Charging ---------^
.                             (SuperSlow)                     |                           |
.                             (Paused)                        |--------> Sleeping --------^
.                                                             |                           |
.                                                             |--------> Charging --------^
*/

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
class BotSingleMcu: public MqttMessageConsumer{
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
            BOT_CHARGING = 8,    //After AGV_MOVING_TO_SOURCE-->AGV_PARKED_AT_SOURCE
            BOT_EMERGENCY_STOPING = 9,
        };

        BotSingleMcu(uint16_t id);
        // GarmentBoxMoverAgent objBoxMoverAgent = GarmentBoxMoverAgent();
        TwinWheelsAgv objAgv = TwinWheelsAgv();

        GcodeQueue* _gcode_queue;
        BoxCarrier objBoxCarrier = BoxCarrier();
        BoxCarrierHardware* objBoxCarrierHardware;
        void Init();
        void SpinOnce() override;
        void ToState(BotSingleMcu::BOT_STATE state);
        void Test(int test_id);
        uint8_t GetMqtt_PubPayload(uint8_t* chars);
        // void onMqttReceived(uint8_t* payload);
        
    protected:
        
    private:
        void ExecuteMqttCommand(const char* command) override;
        RoadGraph objMapNavigator;
        SmartRfidReader objRfid;
        float __battery_voltage;
        void onDetectedMark(uint16_t mapsite_id);
        RoadBranchNode __current_BranchNode;
        uint16_t _ID = 0;
        BotSingleMcu::BOT_STATE __state;
};



#endif