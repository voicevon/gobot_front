#pragma once

#include "CNC/arm_solution/arm_solution_base.h"
#include "core_xa_ab_config.h"


#define VERTICAL_ENDSTOP 15
#define ANGLE_ENDSTOP 2

#define PIN_STEP_ALPHA 5
#define PIN_DIR_ALPHA 19
#define PIN_STEP_BETA 4
#define PIN_DIR_BETA 17



class CncSolution_CoreAZ: public ArmSolutionBase{
    public:
        bool _CutGcodeLine_ToSegmentQueue(Gcode* gcode) override;
        void __ConvertSegment_ToMoveBlockQueue(LineSegment* line) override;

        bool GetCurrentPosition(FkPositionBase* position_fk) override{return false;};  
        float GetDistanceToTarget_FK() override{return 0.0;};
        float GetDistanceToTarget_IK() override;

    protected:

        //Override private
        virtual void IK(FkPositionBase* from_fk,IkPositionBase* to_ik) override;
        virtual void FK(IkPositionBase* ik, FkPositionBase*  to_fk) override;
        virtual void _SetCurrentPositionAsHome(EnumAxis_ForwardKinematic homing_axis) override;

        FkPosition_ZW __current_fk_position;

        Core_XA_ab_config* _config;


};