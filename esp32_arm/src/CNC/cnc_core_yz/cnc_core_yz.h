#pragma once

// #include "CNC/robot_hardware_base.h"
#include "CNC/cnc_base.h"
#include "ESP32Step/src/TeensyStep.h"
#include "CNC/single_axis_homer.h"
#include "box_carrier_hw_config.h"
// #include "MyLibs/board_base.h"
#include "MyBoards/board_base.h"

class Cnc_CoreYZ: public CncBase{
    public:
        Cnc_CoreYZ(){};
        Cnc_CoreYZ(BoardbaseCnc* board, StepControl* stepControl){this->__board=board; this->objStepControl=stepControl;};
        void InitRobot() override;
        void InitMe(BoardbaseCnc* board, StepControl* stepControl){this->__board=board; this->objStepControl=stepControl;};
        void LinkStepper(Stepper* alpha, Stepper* beta);
        void LinkHomer(SingleAxisHomer* homer_z, SingleAxisHomer* homer_y);

        void HomeSingleAxis(char axis) override;
        void RunG1(Gcode* gcode) override;

        bool GetCurrentPosition(FkPositionBase* position_fk) override{return false;};  
        float GetDistanceToTarget_FK() override{return 0.0;};
        float GetDistanceToTarget_IK() override;

    private:
        Stepper* stepper_alpha; // = Stepper(PIN_STEP_ALPHA, PIN_DIR_ALPHA);
        Stepper* stepper_beta; // = Stepper(PIN_STEP_BETA, PIN_DIR_BETA);
        StepControl* objStepControl;

        //Override private
        // void SpinOnce_BaseEnter() override {};
        // void SpinOnce_BaseExit() override {};
        virtual void IK(FkPositionBase* from_fk,IkPositionBase* to_ik) override;
        virtual void FK(IkPositionBase* ik, FkPositionBase*  to_fk) override;

        void RunG6(Gcode* gcode) override{} ; 
        void _running_G1() override;
        void _running_G28() override;
        void RunM123(uint8_t eef_channel, EefAction eef_action) override;
        void RunM84() override;
        std::string GetHomeTrigerStateString() override {return " ";};
        // void __EnableMotor(char actuator, bool enable_it) override;

        FkPosition_YZ __current_fk_position;

        SingleAxisHomer* __homing_helper;
        SingleAxisHomer* objHomeHelper_vertical;
        SingleAxisHomer* objHomeHelper_y;
        BoxCarrierHardwareConfig  __config;
        BoardbaseCnc* __board;

};