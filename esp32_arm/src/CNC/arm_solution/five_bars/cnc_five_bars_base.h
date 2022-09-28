#pragma once


#include <ESP32Servo.h>
#include "CNC/board/cnc_board_base.h"
#include "CNC/arm_solution/arm_solution_base.h"
#include "config_base.h"
#include "Robot/eef/eef_standard_code.h"
#include "CNC/arm_solution/axis_homer/axis_homer.h"
#include "MyLibs/MyFunctions.hpp"

class CncSolution_FiveBarsBase: public ArmSolutionBase{
    public:

    protected:
        CncSolution_FiveBarConfig* _config;

    
    private:
        virtual void IK(FkPositionBase* from_fk, IkPositionBase* to_ik) override;
        virtual void FK(IkPositionBase* from_ik, FkPositionBase* to_fk) override;
        float GetDistanceToTarget_FK() override{return 0.0;};
        float GetDistanceToTarget_IK() override;
        bool GetCurrentPosition(FkPositionBase* position_fk) override {return false;};
        bool _ConvertG1ToLineSegment(Gcode* gcode, LineSegment* line) override;
        // void RunG1(Gcode* gcode) override;
        void RunG6(Gcode* gcode) override {};   //Block mode
        std::string GetHomeTrigerStateString() override {return " ";};
        void _SetCurrentPositionAsHome(EnumAxis homing_axis) override;

        FkPosition_XY __current_fk_position;


};


