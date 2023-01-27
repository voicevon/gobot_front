#pragma once

#include "MyLibs/MyFunctions.hpp"
#include "MyLibs/basic/board_base.h"

#include "CNC/arm_solution/arm_solution_base.h"
#include "Robot/eef/eef_standard_code.h"
#include "MyLibs/component/led_gpio.h"
#include "config_base.h"
#include "Robot/axis_homer/position_trigger.h"


class Scara_ArmSolution: public ArmSolutionBase{
    public:
        // static Scara_ArmSolution& getInstance()
        // {
        //     static Scara_ArmSolution instance; // Guaranteed to be destroyed.
        //                           // Instantiated on first use.
        //     return instance;
        // }
        // void RunG1(Gcode* gcode) override;
        // bool _CutGcodeLine_ToSegmentQueue(Gcode* gcode) override;
        // void __ConvertSegment_ToMoveBlockQueue(LineSegment* line) override;


        // bool GetCurrentPosition(FKPosition_XYZRPW* position_fk) override;
        // float GetDistanceToTarget_FK() override;
        // float GetDistanceToTarget_IK() override;


    protected:
        virtual void FK_to_IK(FKPosition_XYZRPW* from_fk,IKPosition_abgdekl* to_ik) override;
        virtual void IK_to_FK(IKPosition_abgdekl* ik, FKPosition_XYZRPW*  to_fk) override;

        // void RunG6(Gcode* gcode) override {};   //Block mode

        // void _SetCurrentPositionAsHome(EnumAxis_ForwardKinematic homing_axis) override;

        PositionTrigger* __homer;
        FKPosition_XYZRPW __current_fk_position;
        FKPosition_XYZRPW __next_fk_position;
        CncSolution_ScaraConfigBase* _scara_machine;
};