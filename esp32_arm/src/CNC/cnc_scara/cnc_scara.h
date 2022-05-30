#pragma once

// #include <Arduino.h>
// #include "ESP32Step/src/TeensyStep.h"
#include "MyLibs/MyFunctions.hpp"

// #include "MyBoards/board_base.h"
#include "MyBoards/cnc_board_base.h"
#include "CNC/cnc_base.h"
#include "CNC/single_axis_homer.h"
#include "CNC/eef_standard_code.h"
#include "MyLibs/Components/Led.h"
#include "cnc_scara_config.h"


class CncScara: public CncBase{
    public:
        CncScara();
        // static CncScara& getInstance()
        // {
        //     static CncScara instance; // Guaranteed to be destroyed.
        //                           // Instantiated on first use.
        //     return instance;
        // }
        void Init(CncBoardBase* board) override;
        void HomeSingleAxis(char axis) override;
        void RunG1(Gcode* gcode) override;

        bool GetCurrentPosition(FkPositionBase* position_fk) override;
        float GetDistanceToTarget_FK() override;
        float GetDistanceToTarget_IK() override;


    private:
        virtual void IK(FkPositionBase* from_fk,IkPositionBase* to_ik) override;
        virtual void FK(IkPositionBase* ik, FkPositionBase*  to_fk) override;

        void RunG6(Gcode* gcode) override {};   //Block mode
        void _running_G1() override;
        void _running_G28() override;
        std::string GetHomeTrigerStateString() override {return " ";};
        void RunM123(uint8_t eef_channel, EefAction eef_action) override;
        void RunM84() override;
        
        SingleAxisHomer* alpha_homer;  // = SingleAxisHomer(PIN_HOME_ALHPA_2109, LOW);
        SingleAxisHomer* beta_homer;   // = SingleAxisHomer(PIN_HOME_BETA_2109, LOW);

        Stepper* alpha_stepper;
        Stepper* beta_stepper;
        // StepControl objStepControl;

        Stepper* __homing_stepper;
        SingleAxisHomer* __homing_helper;
        FkPosition_XY __current_fk_position;
        FkPosition_XY __next_fk_position;
        GobotHouseHardwareConfig __config;
        
};