#pragma once

#include "MyBoards/cnc_board_base.h"
#include "CNC/solution/cnc_base.h"
#include "MyBoards/cnc_board_base.h"
#include "ESP32Step/src/TeensyStep.h"
#include "CNC/single_axis_homer.h"
#include "cnc_machine.h"

class Cnc_CoreYZ: public CncSolutionBase{
    public:
        Cnc_CoreYZ(){};
        void Init(CncBoardBase* board) override;

        void RunG28(EnumAxis axis) override;
        void RunG1(Gcode* gcode) override;

        bool GetCurrentPosition(FkPositionBase* position_fk) override{return false;};  
        float GetDistanceToTarget_FK() override{return 0.0;};
        float GetDistanceToTarget_IK() override;

    protected:
        CncCoreYZMachine* _cncMachine;
        // Stepper* stepper_alpha; 
        // Stepper* stepper_beta; 

        //Override private
        virtual void IK(FkPositionBase* from_fk,IkPositionBase* to_ik) override;
        virtual void FK(IkPositionBase* ik, FkPositionBase*  to_fk) override;

        void RunG6(Gcode* gcode) override{} ; 
        void _running_G1() override;
        void _running_G28() override;
        // void RunM123(uint8_t eef_channel, EefAction eef_action) override;
        void RunM123(uint8_t eef_channel, uint8_t eef_action) override;
        void RunM84() override;
        std::string GetHomeTrigerStateString() override {return " ";};

        FkPosition_YZ __current_fk_position;

        SingleAxisHomer* __homing_helper;
        SingleAxisHomer* objHomeHelper_vertical;
        SingleAxisHomer* objHomeHelper_y;

};