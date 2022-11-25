#pragma once
// #include "all_devices.h"
#include "all_applications.h"
#ifdef I_AM_CABLEBOT_CORNER


#include "CNC/robot_hardware_base.h"
#include "CNC/Commu/CommuUart.h"
#include "CNC/Commu/CommuBleGattServer.h"
// #include "CNC/Axis/SingleAxis.h"


#include "CNC/Actuator/DCMotor/DcMotor.h"
#include "Robot/Sensor/IrEncoderHelper.h"
// #include "CNC/Actuator/DCMotor/DCDriverHBridge.h"
#include "CNC/Actuator/DCMotor/h_bridge_yuxin.h"
#include "CNC/Actuator/DCMotor/DCMotorController.h"

#include "CNC/HomeHelper.h"
#include "MyLibs/Components/Led.h"

#define PIN_HOME_SENSOR_2130 32

#define PIN_DC_MOTOR_A_2130 27
#define PIN_DC_MOTOR_B_2130 14
#define PIN_LED_POWER_2130 22


class RoomBotCorner: public RobotBase{
    public:
        RoomBotCorner(char axis_name);
        void RunG28(char axis) override;
        void RunG1(Gcode* gcode) override;
        void RunG6(Gcode* gcode) override;
        void Init(BoardbaseCnc* board) override {assert("Must pass me an IrEncoderHelper*");};
        // void Init(IrEncoderHelper* sensorHelper);
        void test_hBridge();
        void test_home();
        // bool GetCurrentPosition(FKPosition_XYZRPY* position_fk) override {assert("Do not invoke me()"); return false;};
        // float GetDistanceToTarget_FK() override{assert("Do not invoke me()"); return 0.0;};
        // float GetDistanceToTarget_IK() override{assert("Do not invoke me()"); return 0.0;};
    protected:
        
    private:
        virtual void IK(FKPosition_XYZRPY* from_fk, IKPosition_abgdekl* to_ik) override;
        virtual void FK(IKPosition_abgdekl* from_ik, FKPosition_XYZRPY* to_fk) override;
        void MoveToTargetPosition();
        // void SpinOnce_BaseEnter() override {};
        void SpinOnce_BaseExit() override;
        Led objLed_power = Led(0,PIN_LED_POWER_2130,LOW);
        Led objLed_home_alpha = Led(1,2,LOW);
        // DCDriverHBridge objHBridge = YuXin_HBridge(PIN_DC_MOTOR_A_2130, PIN_DC_MOTOR_B_2130);
        YuXin_HBridge objHBridge = YuXin_HBridge();
        HomeHelper objHomeTriger = HomeHelper(PIN_HOME_SENSOR_2130, LOW);
        DCMotor objDcMotor = DCMotor();   //parent is CncActuatorBase
        CommuUart objCommuUart = CommuUart();
        CommuBleGattServer objCommuBle = CommuBleGattServer();

        // FkPosX currentPosX;
        FkPosition_X nextPosX;
        // void _running_G1() override{};
        void _running_G28() override{};
        // void RunM123(uint8_t eef_channel, EefAction eef_action) override {};
        void RunM84() override{};
        void __EnableMotor(char actuator, bool enable_it) override {};   
};


#endif
