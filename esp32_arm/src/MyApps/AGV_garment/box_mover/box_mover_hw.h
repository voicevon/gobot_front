#pragma once

#include "Robot/RobotBase.h"
#include "ESP32Step/src/TeensyStep.h"
#include "Robot/HomeHelper.h"
#include "box_mover_hw_config.h"


#define VERTICAL_ENDSTOP 15
#define ANGLE_ENDSTOP 2

#define PIN_STEP_ALPHA 22
#define PIN_DIR_ALPHA 22
#define PIN_STEP_BETA 22
#define PIN_DIR_BETA 22



class BoxMoverHardware:public RobotBase{
    public:
        BoxMoverHardware();

        void Init_Linkage() override;
        void HomeSingleAxis(char axis) override;
        void RunG1(Gcode* gcode) override;

        bool GetCurrentPosition(FkPositionBase* position_fk) override{return false;};  
        void MoveToTargetPosition() override{};
        float GetDistanceToTarget_FK() override{return 0.0;};
        float GetDistanceToTarget_IK() override{return 0.0;};

    private:
        Stepper objStepper_alpha = Stepper(PIN_STEP_ALPHA, PIN_DIR_ALPHA);
        Stepper objStepper_beta = Stepper(PIN_STEP_BETA, PIN_DIR_BETA);
        StepControl objStepControl;

        //Override private
        void SpinOnce_BaseEnter() override {};
        void SpinOnce_BaseExit() override {};
        virtual void IK(FkPositionBase* from_fk,IkPositionBase* to_ik) override;
        virtual void FK(IkPositionBase* ik, FkPositionBase*  to_fk) override;

        void RunG6(Gcode* gcode) override{} ; 
        void _running_G1() override;
        void _running_G28() override;
        std::string GetHomeTrigerStateString() override {return " ";};

        FkPosition_ZW __current_fk_position;

        HomeHelper* __homing_helper;
        HomeHelper objHomeHelper_vertical = HomeHelper(VERTICAL_ENDSTOP, LOW);
        HomeHelper objHomeHelper_angle = HomeHelper(ANGLE_ENDSTOP, LOW);        
        BoxMoverHardwareConfig  __config;
};