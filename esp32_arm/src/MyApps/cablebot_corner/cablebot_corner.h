#pragma once


#include "Robot/RobotBase.h"
#include "Robot/Commu/CommuUart.h"
#include "Robot/Commu/CommuBleGattServer.h"
#include "Robot/Axis/SingleAxis.h"


#include "Robot/Actuator/DCMotor/DcMotor.h"
// #include "SimpleFOC/sensors/Encoder.h"
// #include "Robot/Sensor/IREncoder.h"
#include "Robot/Sensor/IrEncoderHelper.h"
#include "Robot/Actuator/DCMotor/DCDriverHBridge.h"
#include "Robot/Actuator/DCMotor/DCMotorController.h"

#include "Robot/HomeHelper.h"
#include "MyLibs/Components/Led.h"

#define PIN_HOME_SENSOR_2130 15

#define PIN_DC_MOTOR_A_2130 28
#define PIN_DC_MOTOR_B_2130 29
#define PIN_LED_POWER_2130 22


class CableBotCorner: public RobotBase{
    public:
        CableBotCorner(char axis_name);
        void HomeAllAxises() override;
        void RunG1(Gcode* gcode) override;
        // void SpinOnce() override;
        void Init() override {assert("Must pass me an IrEncoderHelper*");};
        void Init(IrEncoderHelper* sensorHelper);
        // SingleAxis singleAxis = SingleAxis();

        // void LinkDriver(DCDriverHBridge* hBridge ){this->hBridge=hBridge;};
        // DCDriverHBridge* hBridge;
    protected:
        ik_position ik(float x, float y) override;
    private:
        void SpinOnce_BaseEnter() override {};
        void SpinOnce_BaseExit() override {};
        Led objLed_power = Led(0,PIN_LED_POWER_2130,LOW);
        Led objLed_home_alpha = Led(1,2,LOW);
        DCDriverHBridge objHBridge = DCDriverHBridge(PIN_DC_MOTOR_A_2130, PIN_DC_MOTOR_B_2130);
        HomeHelper objHomeTriger = HomeHelper(PIN_HOME_SENSOR_2130, HIGH);
        DCMotor objDcMotor = DCMotor();
        CommuUart objCommuUart = CommuUart();
        CommuBleGattServer objCommuBle = CommuBleGattServer();
        SingleAxis singleAxis = SingleAxis();
        // ActuatorControllerBase objActuatorController = ActuatorControllerBase();

        
};

