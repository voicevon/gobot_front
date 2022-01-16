#pragma once
#include "all_devices.h"
#ifdef I_AM_GARMENT_BOT

// #include "Robot/RobotBase.h"
// #include "Robot/Commu/CommuUart.h"


// #include "Robot/Actuator/DCMotor/DcMotor.h"
// #include "Robot/Sensor/IrEncoderHelper.h"
// #include "track_sensor_i2c.h"
// #include "Robot/Actuator/DCMotor/h_bridge_l298n.h"
// #include "Robot/Actuator/DCMotor/DCMotorController.h"

// #include "Robot/HomeHelper.h"
// #include "MyLibs/Components/Led.h"

// #include "agv_garment_21a.h"
#include "agv_slim_hw.h"
#include "box_mover.h"

// #define PIN_LED_POWER_2130 23


/*
.     |<-----------^    |<-----------------------------
.     |            ^    |                             ^
.   Sleeping  --> Agv_Moving  --> Robot_Loading  -----|  
.                       |                             ^
.                       |---------> Robot_Unloading --|
.                       |                             ^
.                       |---------> Charging ---------|
.                       |                             ^
.                       |---------> Stopping ---------|
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
        AgvSlimHardware agv = AgvSlimHardware();
        BoxMover boxMaster = BoxMover();
        // void Init_Linkage(IrEncoderHelper* sensorHelper);
        void test_hBridge();
        void test_home();
        void SpinOnce();
        void SetMode(GARMENTBOT_MODE mode);
        void ExecuteCommand(int topic, int payload);
    protected:
        GARMENTBOT_MODE _State;
        void SpinOnce_Working();
        
    private:
        // L298N objLeftWheelBridge = L298N();
        // L298N objRightWheelBridge = L298N();
        TrackSensor_Shengteng objTrackSensor = TrackSensor_Shengteng();
        int objObstacleDetector;


        // L298N objVerticalBridge = L298N();
        // L298N objAngleBridge = L298N();
        // DualEndActuator objVerticalMover = DualEndActuator();
        // DualEndActuator objAngleMover = DualEndActuator();
};



#endif