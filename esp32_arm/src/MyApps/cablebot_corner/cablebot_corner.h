#pragma once


#include "Robot/RobotBase.h"
#include "Robot/Commu/CommuUart.h"
#include "Robot/Commu/CommuBLE.h"
#include "Robot/Axis/SingleAxis.h"


#include "Robot/Actuator/DCMotor/DcMotor.h"
#include "SimpleFOC/sensors/Encoder.h"
#include "Robot/Actuator/DCMotor/DCDriverHBridge.h"
#include "Robot/Actuator/DCMotor/DCMotorController.h"

#include "Robot/HomeTriger.h"
#include "MyLibs/Components/Led.h"

#define PIN_HOME_SENSOR 5
#define PIN_ENCODER_A 16
#define PIN_ENCODER_B 17
#define PIN_DC_MOTOR_A 8
#define PIN_DC_MOTOR_B 9
#define PIN_LED_POWER 22


class CableBotCorner: public RobotBase{
    public:
        CableBotCorner(char axis_name);
        void HomeAllAxises() override;
        void RunG1(Gcode* gcode) override;
        void SpinOnce() override;
        void Init() override;
        // SingleAxis singleAxis = SingleAxis();

        void LinkDriver(DCDriverHBridge* hBridge ){this->hBridge=hBridge;};
        DCDriverHBridge* hBridge;

    private:
        Led objLed_power = Led(0,PIN_LED_POWER,LOW);
        Led objLed_home_alpha = Led(1,2,LOW);
        DCDriverHBridge objBridge = DCDriverHBridge(PIN_DC_MOTOR_A, PIN_DC_MOTOR_B);
        HomeTriger objHomeTriger = HomeTriger(PIN_HOME_SENSOR, HIGH);
        DCMotor objMotor = DCMotor();
        CommuUart commuUart = CommuUart();
        CommuBle commuBle = CommuBle();
        SingleAxis singleAxis = SingleAxis();
        // ActuatorControllerBase objActuatorController = ActuatorControllerBase();

        
};

