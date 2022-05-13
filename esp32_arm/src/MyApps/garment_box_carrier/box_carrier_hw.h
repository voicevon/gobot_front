#pragma once

#include "Robot/robot_hardware_base.h"
#include "ESP32Step/src/TeensyStep.h"
#include "Robot/HomeHelper.h"
#include "box_carrier_hw_config.h"

#include "Robot/Commu/CommuUart.h"


#define VERTICAL_ENDSTOP 15
#define Y_ENDSTOP 2

#define PIN_STEP_ALPHA 5
#define PIN_DIR_ALPHA 19
#define PIN_STEP_BETA 4
#define PIN_DIR_BETA 17


class BoxCarrierHardware:public RobotBase{
    public:
        BoxCarrierHardware();
        BoxCarrierHardware(Adafruit_MCP23X17* mcp_23018);

        void InitRobot() override;
        void HomeSingleAxis(char axis) override;
        void RunG1(Gcode* gcode) override;

        bool GetCurrentPosition(FkPositionBase* position_fk) override{return false;};  
        float GetDistanceToTarget_FK() override{return 0.0;};
        float GetDistanceToTarget_IK() override;

    private:
        Stepper objStepper_alpha = Stepper(PIN_STEP_ALPHA, PIN_DIR_ALPHA);
        Stepper objStepper_beta = Stepper(PIN_STEP_BETA, PIN_DIR_BETA);
        StepControl objStepControl;

        //Override private
        void SpinOnce_BaseEnter() override {};
        // void SpinOnce_BaseExit() override {};
        virtual void IK(FkPositionBase* from_fk,IkPositionBase* to_ik) override;
        virtual void FK(IkPositionBase* ik, FkPositionBase*  to_fk) override;

        void RunG6(Gcode* gcode) override{} ; 
        void _running_G1() override;
        void _running_G28() override;
        void RunM123(uint8_t eef_channel, EefAction eef_action) override;
        void RunM84() override;
        std::string GetHomeTrigerStateString() override {return " ";};
        void __EnableMotor(char actuator, bool enable_it) override;

        FkPosition_YZ __current_fk_position;

        HomeHelper* __homing_helper;
        HomeHelper* objHomeHelper_vertical;
        HomeHelper* objHomeHelper_y;
        BoxCarrierHardwareConfig  __config;
        Adafruit_MCP23X17* __mcp23018;

};