#pragma once
#include "all_devices.h"
#ifdef I_AM_GARMENT_BOT

#include "Robot/RobotBase.h"
#include "Robot/Commu/CommuUart.h"


#include "Robot/Actuator/DCMotor/DcMotor.h"
#include "Robot/Sensor/IrEncoderHelper.h"
// #include "track_sensor_i2c.h"
#include "Robot/Actuator/DCMotor/h_bridge_l298n.h"
#include "Robot/Actuator/DCMotor/DCMotorController.h"

#include "Robot/HomeHelper.h"
#include "MyLibs/Components/Led.h"

#include "agv_garment_21a.h"
#include "box_mover_21a.h"

#define PIN_LED_POWER_2130 23


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
        void Init();
        AgvGarment_21a agv_21a = AgvGarment_21a();
        BoxMover_21a robot_21a = BoxMover_21a();
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
        // DCMotor objLeftWheel = DCMotor();   //parent is ActuatorBase
        // DCMotor objRightWheel = DCMotor();   //parent is ActuatorBase
        DCMotor objZMotor = DCMotor();   //parent is ActuatorBase
        DCMotor objAngleMotor = DCMotor();   //parent is ActuatorBase
        TrackSensor_I2C objTrackSensor_i2c = TrackSensor_I2C();

        HomeHelper objHomeTriger = HomeHelper(PIN_HOME_SENSOR_2130, LOW);
        CommuUart objCommuUart = CommuUart();
};



#endif