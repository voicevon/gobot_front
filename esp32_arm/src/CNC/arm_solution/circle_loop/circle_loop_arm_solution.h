#pragma once

#include "CNC/board/cnc_board_base.h"
#include "CNC/arm_solution/arm_solution_base.h"
#include "circle_loop_arm_solution_config.h"
#include "CNC/planner/planner.h"

//This is a virtual class. No Constructor.
class CircleLoop_ArmSolution: public ArmSolutionBase{
    public:
        // void Init(CncBoardBase* board) override;
        void RunG1(Gcode* gcode) override;

        bool GetCurrentPosition(FkPositionBase* position_fk) override{return false;};  
        float GetDistanceToTarget_FK() override{return 0.0;};
        float GetDistanceToTarget_IK() override;

    protected:
        CircleLoop_KinematicConfig _kinematic_config;  
        void _SetCurrentPositionAsHome(EnumAxis homing_axis) override;
        EnumAxis _AXIS;   // TODO: doubel check this var.

    private:
        virtual void IK(FkPositionBase* from_fk,IkPositionBase* to_ik) override;
        virtual void FK(IkPositionBase* ik, FkPositionBase*  to_fk) override;

        void RunG6(Gcode* gcode) override{} ; 
        std::string GetHomeTrigerStateString() override {return " ";};

        FkPosition_A __current_fk_position;

};