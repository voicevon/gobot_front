#pragma once


#include <ESP32Servo.h>
#include "MyBoards/cnc_board_base.h"
#include "../solution_base.h"
#include "config_base.h"
#include "Robot/eef/eef_standard_code.h"
#include "Robot/homer/single_axis_homer.h"
#include "MyLibs/MyFunctions.hpp"

class CncFiveBarsBase: public CncSolutionBase{
    public:
        void Init(CncBoardBase* board) override;
        void RunG1(Gcode* gcode) override;
        bool GetCurrentPosition(FkPositionBase* position_fk) override {return false;};

    protected:
        CncFiveBarConfig* _fivebarMachine;

    
    private:
        virtual void IK(FkPositionBase* from_fk, IkPositionBase* to_ik) override;
        virtual void FK(IkPositionBase* from_ik, FkPositionBase* to_fk) override;
        float GetDistanceToTarget_FK() override{return 0.0;};
        float GetDistanceToTarget_IK() override;

        bool homed;

        void RunG6(Gcode* gcode) override {};   //Block mode
        std::string GetHomeTrigerStateString() override {return " ";};
        void _SetCurrentPositionAsHome(EnumAxis homing_axis) override;
        FkPosition_XY __current_fk_position;


};


