#pragma once


#include "CNC/arm_solution/arm_solution_base.h"
#include "config_base.h"
// #include "Robot/eef/eef_standard_code.h"
// #include "CNC/arm_solution/axis_homer/axis_homer.h"
#include "MyLibs/MyFunctions.hpp"

class FiveBars_ArmSolution: public ArmSolutionBase{
    public:

    protected:
        CncSolution_FiveBarConfig* _config;  //TODO:: remove-able?

    
    private:
        virtual void IK(FkPositionBase* from_fk, IkPositionBase* to_ik) override;
        virtual void FK(IkPositionBase* from_ik, FkPositionBase* to_fk) override;
        float GetDistanceToTarget_FK() override{return 0.0;};
        float GetDistanceToTarget_IK() override;
        bool GetCurrentPosition(FkPositionBase* position_fk) override {return false;};
        bool _CutGcodeLine_ToSegmentQueue(Gcode* gcode) override;
        void __ConvertSegment_ToMoveBlockQueue(LineSegment* line) override;
        
        std::string GetHomeTrigerStateString() override {return " ";}; //TODO:: remove 
        void _SetCurrentPositionAsHome(EnumAxis_ForwardKinematic homing_axis) override;

        FkPosition_XY __current_fk_position;


};


