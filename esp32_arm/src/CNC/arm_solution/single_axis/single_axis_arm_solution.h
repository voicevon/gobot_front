#pragma once

#include "CNC/board/cnc_board_base.h"
#include "CNC/arm_solution/arm_solution_base.h"
#include "kinematic_config.h"
#include "CNC/planner/queue_line_segment.h"
#include "CNC/planner/queue_move_block.h"


//This is a virtual class. No Constructor.
class SingleAxis_ArmSolution: public ArmSolutionBase{
    public:
        bool _CutGcodeLine_ToSegmentQueue(Gcode* gcode) override;
        void __ConvertSegment_ToMoveBlockQueue(LineSegment* line)override;


        bool GetCurrentPosition(FkPositionBase* position_fk) override{return false;};  
        float GetDistanceToTarget_FK() override{return 0.0;};
        float GetDistanceToTarget_IK() override;

    protected:
        SingleAxis_KinematicConfig* _config;  
        void _SetCurrentPositionAsHome(EnumAxis_ForwardKinematic homing_axis) override;
        EnumAxis_ForwardKinematic _AXIS;   // TODO: doubel check this var.

    private:
        virtual void IK(FkPositionBase* from_fk,IkPositionBase* to_ik) override;
        virtual void FK(IkPositionBase* ik, FkPositionBase*  to_fk) override;

        // void RunG6(Gcode* gcode) override{} ; 
        std::string GetHomeTrigerStateString() override {return " ";};

        FkPosition_A __current_fk_position;

};