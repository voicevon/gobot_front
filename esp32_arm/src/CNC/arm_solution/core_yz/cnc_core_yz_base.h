#pragma once

#include "CNC/board/cnc_board_base.h"
#include "CNC/arm_solution/arm_solution_base.h"
#include "CNC/arm_solution/axis_homer/axis_homer.h"
#include "ESP32Step/src/TeensyStep.h"
#include "config_base.h"

class CncSolution_CoreYZBase: public ArmSolutionBase{
    public:
        // void Init(CncBoardBase* board) override;
        // void RunG1(Gcode* gcode) override;
        bool _CutGcodeLine_ToSegmentQueue(Gcode* gcode) override;
        void __ConvertSegment_ToMoveBlockQueue(LineSegment* line) override;

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

        // void RunG6(Gcode* gcode) override{} ; 
        std::string GetHomeTrigerStateString() override {return " ";};

        FkPosition_YZ __current_fk_position;

        PositionTrigger* __homing_helper;
        PositionTrigger* objHomeHelper_vertical;
        PositionTrigger* objHomeHelper_y;

};