#pragma once


#include "CNC/arm_solution/arm_solution_base.h"
#include "config_base.h"
#include "MyLibs/MyFunctions.hpp"

class FiveBars_ArmSolution: public ArmSolutionBase{
    public:

    protected:
        CncSolution_FiveBarConfig* _config;  //TODO:: remove-able?

    
    private:
        virtual void IK(FKPosition_XYZRPY* from_fk, IKPosition_abgdekl* to_ik) override;
        virtual void FK(IKPosition_abgdekl* from_ik, FKPosition_XYZRPY* to_fk) override;
        float GetDistanceToTarget_FK() override{return 0.0;};
        float GetDistanceToTarget_IK() override;
        bool GetCurrentPosition(FKPosition_XYZRPY* position_fk) override {return false;};
        bool _CutGcodeLine_ToSegmentQueue(Gcode* gcode) override;
        void __ConvertSegment_ToMoveBlockQueue(LineSegment* line) override;
        
        void _SetCurrentPositionAsHome(EnumAxis_ForwardKinematic homing_axis) override;

        FKPosition_XYZRPY __current_fk_position;


};


