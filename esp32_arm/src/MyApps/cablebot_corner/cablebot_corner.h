#pragma once


#include "Robot/RobotBase.h"
#include "Robot/Commu/CommuUart.h"
#include "Robot/Commu/CommuBleGattServer.h"
#include "Robot/Axis/SingleAxis.h"


#include "Robot/Actuator/DCMotor/DcMotor.h"
#include "SimpleFOC/sensors/Encoder.h"
#include "Robot/Actuator/DCMotor/DCDriverHBridge.h"
#include "Robot/Actuator/DCMotor/DCMotorController.h"

#include "Robot/HomeHelper.h"
#include "MyLibs/Components/Led.h"

#define PIN_HOME_SENSOR_2130 5
#define PIN_ENCODER_A_2130 16
#define PIN_ENCODER_B_2130 17
#define PIN_DC_MOTOR_A_2130 8
#define PIN_DC_MOTOR_B_2130 9
#define PIN_LED_POWER_2130 22


class CableBotCorner: public RobotBase{
    public:
        CableBotCorner(char axis_name);
        void HomeAllAxises() override;
        void RunG1(Gcode* gcode) override;
        // void SpinOnce() override;
        void Init() override {assert("Must pass me an encoder!");};
        void Init(Encoder* encoder);
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

