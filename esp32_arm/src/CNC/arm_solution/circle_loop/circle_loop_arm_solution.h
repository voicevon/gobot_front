#pragma once

#include "CNC/board/cnc_board_base.h"
#include "CNC/arm_solution/arm_solution_base.h"
#include "circle_loop_arm_solution_config.h"
#include "CNC/planner/planner.h"

class CircleLoop_ArmSolution: public ArmSolutionBase{
    public:
        bool _CutGcodeLine_ToSegmentQueue(Gcode* gcode) override;
        void __ConvertSegment_ToMoveBlockQueue(LineSegment* line) override;


        bool GetCurrentPosition(FKPosition_XYZRPY* position_fk) override{return false;};  
        float GetDistanceToTarget_FK() override{return 0.0;};
        float GetDistanceToTarget_IK() override;

    protected:
        CircleLoop_KinematicConfig _kinematic_config;  
        void _SetCurrentPositionAsHome(EnumAxis_ForwardKinematic homing_axis) override;
        EnumAxis_ForwardKinematic _AXIS;   // TODO: doubel check this var.

    private:
        // EnumAxis ConvertToEnum(char axis) override{return AXIS_ALPHA;};
        void IK(FKPosition_XYZRPY* from_fk,IKPosition_abgdekl* to_ik) override;
        void FK(IKPosition_abgdekl* ik, FKPosition_XYZRPY*  to_fk) override;

        // void RunG6(Gcode* gcode) override{} ; 

        FKPosition_XYZRPY __current_fk_position;
        FKPosition_XYZRPY __target_fk_position;


};