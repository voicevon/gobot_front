#pragma once

#include "MyBoards/cnc_board_base.h"
#include "../solution_base.h"
#include "ESP32Step/src/TeensyStep.h"
// #include "Robot/homer/single_axis_homer.h"
#include "Robot/position_trigger/position_triggers.h"
#include "config_base.h"

class CncSolution_CoreYZBase: public CncSolutionBase{
    public:
        // void Init(CncBoardBase* board) override;
        void RunG1(Gcode* gcode) override;
        bool GetCurrentPosition(FkPositionBase* position_fk) override{return false;};  
        float GetDistanceToTarget_FK() override{return 0.0;};
        float GetDistanceToTarget_IK() override;

    protected:
        CncSolution_CoreYZConfigBase* _cncMachine;

        //Override private
        virtual void IK(FkPositionBase* from_fk,IkPositionBase* to_ik) override;
        virtual void FK(IkPositionBase* ik, FkPositionBase*  to_fk) override;
        void RunG28_CombinedAxis(EnumAxis axis);
        virtual void _SetCurrentPositionAsHome(EnumAxis homing_axis) override;

        void RunG6(Gcode* gcode) override{} ; 
        std::string GetHomeTrigerStateString() override {return " ";};

        FkPosition_YZ __current_fk_position;

        SinglePositionTriger* __homing_helper;
        SinglePositionTriger* objHomeHelper_vertical;
        SinglePositionTriger* objHomeHelper_y;

};