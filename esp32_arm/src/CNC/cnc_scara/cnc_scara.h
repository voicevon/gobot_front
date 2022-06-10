#pragma once

#include "MyLibs/MyFunctions.hpp"
#include "MyBoards/cnc_board_base.h"
#include "CNC/cnc_base.h"
#include "CNC/single_axis_homer.h"
#include "CNC/eef_standard_code.h"
#include "MyLibs/Components/Led.h"
#include "cnc_machine.h"


class CncScara: public CncBase{
    public:
        CncScara();
        // static CncScara& getInstance()
        // {
        //     static CncScara instance; // Guaranteed to be destroyed.
        //                           // Instantiated on first use.
        //     return instance;
        // }
        void Init(CncBoardBase* board, CncMachineBase* config) override;
        void HomeSingleAxis(char axis) override;
        void RunG1(Gcode* gcode) override;

        bool GetCurrentPosition(FkPositionBase* position_fk) override;
        float GetDistanceToTarget_FK() override;
        float GetDistanceToTarget_IK() override;


    protected:
        virtual void IK(FkPositionBase* from_fk,IkPositionBase* to_ik) override;
        virtual void FK(IkPositionBase* ik, FkPositionBase*  to_fk) override;

        void RunG6(Gcode* gcode) override {};   //Block mode
        void _running_G1() override;
        void _running_G28() override;
        std::string GetHomeTrigerStateString() override {return " ";};
        void RunM123(uint8_t eef_channel, EefAction eef_action) override;
        void RunM84() override;
        
        SingleAxisHomer* alpha_homer; 
        SingleAxisHomer* beta_homer;   

        Stepper* alpha_stepper;
        Stepper* beta_stepper;

        Stepper* __homing_stepper;
        SingleAxisHomer* __homing_helper;
        FkPosition_XY __current_fk_position;
        FkPosition_XY __next_fk_position;
        CncScaraMachine* _scara_machine;
};