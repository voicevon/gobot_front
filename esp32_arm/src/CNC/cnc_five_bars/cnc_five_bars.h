#pragma once


#include <ESP32Servo.h>
#include "MyBoards/cnc_board_base.h"
#include "CNC/cnc_base.h"
#include "cnc_five_bar_machine.h"
#include "CNC/eef_standard_code.h"
#include "CNC/single_axis_homer.h"
#include "MyLibs/MyFunctions.hpp"
#include "CNC/Robot_eef_base.h"

class CncFiveBars: public CncBase{
    public:
        CncFiveBars(){};
        void HomeSingleAxis(char axis) override;
        void RunG1(Gcode* gcode) override;
        void Init(CncBoardBase* board, CncMachineBase* config) override;
        void LinkEef(RobotEefBase* eef){this->__eef=eef;};

        bool GetCurrentPosition(FkPositionBase* position_fk) override {return false;};
        void Calibrate(int step,bool enable_eef_coil);
        void RunM84() override;

    protected:
        CncFiveBarMachine* _fivebarMachine;

    
    private:
        virtual void IK(FkPositionBase* from_fk, IkPositionBase* to_ik) override;
        virtual void FK(IkPositionBase* from_ik, FkPositionBase* to_fk) override;
        float GetDistanceToTarget_FK() override{return 0.0;};
        float GetDistanceToTarget_IK() override;

        // Servo* eefServo;
        bool homed;

        void RunG6(Gcode* gcode) override {};   //Block mode
        void RunM123(uint8_t eef_channel, EefAction eef_action) override;
        std::string GetHomeTrigerStateString() override {return " ";};
        void _running_G1() override;
        void _running_G28() override;
    
        SingleAxisHomer* alpha_homer;
        SingleAxisHomer* beta_homer;

        Stepper* alpha_stepper;
        Stepper* beta_stepper;
        StepControl objStepControl;

        Stepper* __homing_stepper;
        SingleAxisHomer* __current_homer;
        FkPosition_XY __current_fk_position;
        // BoardbaseCnc* __board;
        RobotEefBase* __eef;


};


