#pragma once

#include "CNC/arm_solution/arm_solution_base.h"
#include "config_base.h"

class CncSolution_CoreXA: public ArmSolutionBase{
    public:
        bool _CutGcodeLine_ToSegmentQueue(Gcode* gcode) override;
        void __ConvertSegment_ToMoveBlockQueue(LineSegment* line) override{};


    protected:
        void _SetCurrentPositionAsHome(EnumAxis_ForwardKinematic homing_axis) override{};

        //Override private
        void IK(FkPositionBase* from_fk,IkPositionBase* to_ik) override;
        void FK(IkPositionBase* ik, FkPositionBase*  to_fk) override;
        bool GetCurrentPosition(FkPositionBase* position_fk) override{return false;};  
        float GetDistanceToTarget_FK() override{return 0.0;};
        float GetDistanceToTarget_IK() override{};

        FkPosition_ZW __current_fk_position;
        CncSolution_CoreAZConfigBase* _config;


};