#pragma once
#include "all_devices.h"
#ifdef I_AM_GARMENT_BOT

#include "Robot/RobotBase.h"
#include "Robot/Commu/CommuUart.h"
// #include "Robot/Commu/CommuBleGattServer.h"
// #include "Robot/Axis/SingleAxis.h"


#include "Robot/Actuator/DCMotor/DcMotor.h"
#include "Robot/Sensor/IrEncoderHelper.h"
// #include "Robot/Actuator/DCMotor/DCDriverHBridge.h"
#include "Robot/Actuator/DCMotor/h_bridge_l298n.h"
#include "Robot/Actuator/DCMotor/DCMotorController.h"

#include "Robot/HomeHelper.h"
#include "MyLibs/Components/Led.h"


#define PIN_LED_POWER_2130 22

#include "Agv.h"
#include "box_mover.h"



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

enum GARMENTBOT_MODE{
    SLEEP = 0,
    CHARGING = 1,
    WORKING = 2,
};

class GarmentBot{
    public:
        GarmentBot();
        AGV agv = AGV();
        BoxMover robot = BoxMover();
        // void HomeAllAxises() override;
        // void RunG1(Gcode* gcode) override;
        // void RunG6(Gcode* gcode) override;
        // void Init_Linkage() override {assert("Must pass me an IrEncoderHelper*");};
        void Init_Linkage(IrEncoderHelper* sensorHelper);
        void test_hBridge();
        void test_home();
        void SpinOnce();
        void SetMode(GARMENTBOT_MODE mode);

    protected:
        GARMENTBOT_MODE _mode;
        void SpinOnce_Working();
    private:
        L298N objLeftWheelBridge = L298N();
        L298N objRightWheelBridge = L298N();
        L298N objZAxisBridge = L298N();
        L298N objAngleBridge = L298N();
        DCMotor objLeftWheel = DCMotor();   //parent is ActuatorBase
        DCMotor obRightWheel = DCMotor();   //parent is ActuatorBase
        DCMotor objZMotor = DCMotor();   //parent is ActuatorBase
        DCMotor objAngleMotor = DCMotor();   //parent is ActuatorBase

        HomeHelper objHomeTriger = HomeHelper(PIN_HOME_SENSOR_2130, LOW);
        CommuUart objCommuUart = CommuUart();
        // CommuBleGattServer objCommuBle = CommuBleGattServer();
};



#endif