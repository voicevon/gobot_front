#pragma once

#include "CNC/arm_solution/arm_solution_base.h"
#include "config_base.h"

class CoreYZ_ArmSolution: public ArmSolutionBase{
    public:
        // bool _CutGcodeLine_ToSegmentQueue(GcodeText* gcode) override;
        // void __ConvertSegment_ToMoveBlockQueue(LineSegment* line) override;

        // bool GetCurrentPosition(FKPosition_XYZRPW* position_fk) override{return false;};  
        // float GetDistanceToTarget_FK() override{return 0.0;};
        // float GetDistanceToTarget_IK() override;

    protected:
        CncSolution_CoreYZConfigBase* _cncMachine;

        //Override private
        virtual void FK_to_IK(FKPosition_XYZRPW* from_fk,IKPosition_abgdekl* to_ik) override;
        virtual void IK_to_FK(IKPosition_abgdekl* ik, FKPosition_XYZRPW*  to_fk) override;
        // virtual void _SetCurrentPositionAsHome(EnumAxis_ForwardKinematic homing_axis) override;


        FKPosition_XYZRPW __current_fk_position;

};