#pragma once

#include "CNC/cnc_base.h"
#include "ESP32Step/src/TeensyStep.h"
#include "CNC/single_axis_homer.h"

#include "spring_maker_hw_config.h"


#define PIN_ANGLE_ENDSTOP 15

#define PIN_STEP_ALPHA 5
#define PIN_DIR_ALPHA 19



class SpringMakerHardware:public CncBase{
    public:
        SpringMakerHardware();

        void InitRobot() override;
        void HomeSingleAxis(char axis) override;
        void RunG1(Gcode* gcode) override;

        bool GetCurrentPosition(FkPositionBase* position_fk) override{return false;};  
        float GetDistanceToTarget_FK() override{return 0.0;};
        float GetDistanceToTarget_IK() override;

    private:
        Stepper objStepper_alpha = Stepper(PIN_STEP_ALPHA, PIN_DIR_ALPHA);
        StepControl objStepControl;

        //Override private
        // void SpinOnce_BaseEnter() override {};
        virtual void IK(FkPositionBase* from_fk,IkPositionBase* to_ik) override;
        virtual void FK(IkPositionBase* ik, FkPositionBase*  to_fk) override;

        void RunG6(Gcode* gcode) override{} ; 
        void _running_G1() override;
        void _running_G28() override;
        void RunM123(uint8_t eef_channel, EefAction eef_action) override;
        void RunM84() override;
        std::string GetHomeTrigerStateString() override {return " ";};
        void __EnableMotor(char actuator, bool enable_it) override;

        FkPosition_A __current_fk_position;

        SingleAxisHomer* __homing_helper;
        SingleAxisHomer objHomeHelper_alpha = SingleAxisHomer(PIN_ANGLE_ENDSTOP, LOW);
        SpringMakerHardwareConfig  __config;

};