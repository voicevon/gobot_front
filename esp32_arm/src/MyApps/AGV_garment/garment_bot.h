#pragma once
#include "all_devices.h"
#ifdef I_AM_AGV_GARMENT

#include "Robot/RobotBase.h"
#include "Robot/Commu/CommuUart.h"
#include "Robot/Commu/CommuBleGattServer.h"
// #include "Robot/Axis/SingleAxis.h"


#include "Robot/Actuator/DCMotor/DcMotor.h"
#include "Robot/Sensor/IrEncoderHelper.h"
#include "Robot/Actuator/DCMotor/DCDriverHBridge.h"
#include "Robot/Actuator/DCMotor/DCMotorController.h"

#include "Robot/HomeHelper.h"
#include "MyLibs/Components/Led.h"

#define PIN_HOME_SENSOR_2130 32

#define PIN_LEFT_WHEEL_DC_MOTOR_A 27
#define PIN_LEFT_WHEEL_DC_MOTOR_B 14
#define PIN_RIGHT_WHEEL_DC_MOTOR_A 27
#define PIN_RIGHT_WHEEL_DC_MOTOR_B 14

#define PIN_Z_DC_MOTOR_A 11
#define PIN_Z_DC_MOTOR_B 11

#define PIN_ANGLE_DC_MOTOR_A 11
#define PIN_ANGLE_DC_MOTORB 11

#define PIN_LED_POWER_2130 22

#include "Agv.h"
#include "box_mover.h"

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

    protected:
        
    private:
        DCDriverHBridge objLeftWheelBridge = DCDriverHBridge(PIN_LEFT_WHEEL_DC_MOTOR_A, PIN_LEFT_WHEEL_DC_MOTOR_B);
        DCDriverHBridge objRightWheelBridge = DCDriverHBridge(PIN_LEFT_WHEEL_DC_MOTOR_A, PIN_LEFT_WHEEL_DC_MOTOR_B);
        DCDriverHBridge objZAxisBridge = DCDriverHBridge(PIN_LEFT_WHEEL_DC_MOTOR_A, PIN_LEFT_WHEEL_DC_MOTOR_B);
        DCDriverHBridge objAngleBridge = DCDriverHBridge(PIN_LEFT_WHEEL_DC_MOTOR_A, PIN_LEFT_WHEEL_DC_MOTOR_B);
        DCMotor objLeftWheel = DCMotor();   //parent is ActuatorBase
        DCMotor obRightWheel = DCMotor();   //parent is ActuatorBase
        DCMotor objZMotor = DCMotor();   //parent is ActuatorBase
        DCMotor objAngleMotor = DCMotor();   //parent is ActuatorBase

        HomeHelper objHomeTriger = HomeHelper(PIN_HOME_SENSOR_2130, LOW);
        CommuUart objCommuUart = CommuUart();
        CommuBleGattServer objCommuBle = CommuBleGattServer();
};



#endif