#pragma once

#include "MyBoards/cnc_board_base.h"
#include "CNC/solution/cnc_base.h"
// #include "cnc_single_axis_config.h"
#include "cnc_single_axis_machine.h"

class CncSingleAxis: public CncBase{
    public:
        CncSingleAxis();

        void Init(CncBoardBase* board, CncMachineBase* config) override;
        void HomeSingleAxis(char axis) override;
        void RunG1(Gcode* gcode) override;

        bool GetCurrentPosition(FkPositionBase* position_fk) override{return false;};  
        float GetDistanceToTarget_FK() override{return 0.0;};
        float GetDistanceToTarget_IK() override;

    protected:
        CncSingleAxis_Machine* _singleAxisConfig;  //TODO:  uinify name:  cncConfig??

    private:
        Stepper* objStepper_alpha;
        StepControl* objStepControl;

        //Override private
        // void SpinOnce_BaseEnter() override {};
        virtual void IK(FkPositionBase* from_fk,IkPositionBase* to_ik) override;
        virtual void FK(IkPositionBase* ik, FkPositionBase*  to_fk) override;

        void RunG6(Gcode* gcode) override{} ; 
        void _running_G1() override;
        void _running_G28() override;
        // void RunM123(uint8_t eef_channel, EefAction eef_action) override;
        void RunM123(uint8_t eef_channel, uint8_t eef_action) override;
        void RunM84() override;
        std::string GetHomeTrigerStateString() override {return " ";};
        // void __EnableMotor(char actuator, bool enable_it) override;

        FkPosition_A __current_fk_position;

        SingleAxisHomer* __homing_helper;
        SingleAxisHomer* objHomeHelper_alpha;

};