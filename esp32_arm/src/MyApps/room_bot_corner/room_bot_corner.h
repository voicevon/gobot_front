#pragma once


#include "Robot/RobotBase.h"
#include "Robot/Commu/CommuUart.h"
#include "Robot/Commu/CommuBleGattServer.h"
#include "Robot/Axis/SingleAxis.h"


#include "Robot/Actuator/DCMotor/DcMotor.h"
#include "Robot/Sensor/IrEncoderHelper.h"
#include "Robot/Actuator/DCMotor/DCDriverHBridge.h"
#include "Robot/Actuator/DCMotor/DCMotorController.h"

#include "Robot/HomeHelper.h"
#include "MyLibs/Components/Led.h"

#define PIN_HOME_SENSOR_2130 15

#define PIN_DC_MOTOR_A_2130 28
#define PIN_DC_MOTOR_B_2130 29
#define PIN_LED_POWER_2130 22


class RoomBotCorner: public RobotBase{
    public:
        RoomBotCorner(char axis_name);
        void HomeAllAxises() override;
        void RunG1(Gcode* gcode) override;
        void Init_Linkage() override {assert("Must pass me an IrEncoderHelper*");};
        void Init_Linkage(IrEncoderHelper* sensorHelper);

    protected:
        
    private:
        virtual IkPositionBase* IK(FkPositionBase* fk) override;
        virtual FkPositionBase* FK(IkPositionBase* ik) override;
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
        FkPosX objFkpos;
        IkPosX objIkPos;
};

