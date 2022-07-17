#pragma once

#include "MyLibs/MyFunctions.hpp"
#include "MyBoards/cnc_board_base.h"
#include "../solution_base.h"
// #include "CNC/single_axis_homer.h"
#include "Robot/homer/single_axis_homer.h"
// #include "robot_eef/eef_standard_code.h"
#include "Robot/eef/eef_standard_code.h"
#include "MyLibs/Components/Led.h"
// #include "cnc_machine.h"
#include "config_base.h"


class CncScaraSolution: public CncSolutionBase{
    public:
        // static CncScaraSolution& getInstance()
        // {
        //     static CncScaraSolution instance; // Guaranteed to be destroyed.
        //                           // Instantiated on first use.
        //     return instance;
        // }
        // void Init(CncBoardBase* board) override;
        // void RunG28(EnumAxis axis) override;
        void RunG1(Gcode* gcode) override;

        bool GetCurrentPosition(FkPositionBase* position_fk) override;
        float GetDistanceToTarget_FK() override;
        float GetDistanceToTarget_IK() override;


    protected:
        virtual void IK(FkPositionBase* from_fk,IkPositionBase* to_ik) override;
        virtual void FK(IkPositionBase* ik, FkPositionBase*  to_fk) override;

        void RunG6(Gcode* gcode) override {};   //Block mode
        // void _running_G1() override;
        // void _running_G28() override;
        std::string GetHomeTrigerStateString() override {return " ";};
        // void RunM123(uint8_t eef_channel, EefAction eef_action) override;
        // void RunM123(uint8_t eef_channel, uint8_t eef_action) override;
        // void RunM84() override;

        void _SetCurrentPositionAsHome(EnumAxis homing_axis) override;

        SingleAxisHomer* __homer;
        FkPosition_XY __current_fk_position;
        FkPosition_XY __next_fk_position;
        CncSolution_ScaraConfigBase* _scara_machine;
};