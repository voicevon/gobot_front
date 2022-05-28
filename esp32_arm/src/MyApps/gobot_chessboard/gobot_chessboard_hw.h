#pragma once


#include <ESP32Servo.h>
// #include "Robot/robot_hardware_base.h"
#include "Robot/cnc_base.h"
#include "ESP32Step/src/TeensyStep.h"
#include "Robot/Commu/CommuUart.h"
#include "MyLibs/MyFunctions.hpp"
// #include "Robot/Gcode.h"

#include "gobot_chessboard_hw_config.h"
// #include "Robot/HomeHelper.h"
#include "Robot/single_axis_homer.h"
#include "MyLibs/Components/Led.h"
#include "Robot/eef_standard_code.h"



/*
What I know:    
        X,Y position.

What I don't know:
        Chessboard size, and where is chessboard. 
        Stone point position,
        Trashbin position,

How to solve the concepts I don't know?
    Please send X,Y position to me.
    Don't send me nominal name of go game (either of any other name).

*/
class GobotChessboardHardware: public CncBase{
    public:
        // static GobotChessboardHardware& getInstance()
        // {
        //     static GobotChessboardHardware instance; // Guaranteed to be destroyed.
        //                           // Instantiated on first use.
        //     return instance;
        // }
        GobotChessboardHardware(){};
        void HomeSingleAxis(char axis) override;
        void RunG1(Gcode* gcode) override;
        void InitRobot() override;
        void LinkHomer(SingleAxisHomer* alpha_homer, SingleAxisHomer* beta_homer){this->alpha_homer=alpha_homer; this->beta_homer=beta_homer;};
        void LinkStepper(Stepper* alpha, Stepper* beta){this->alpha_stepper=alpha; this->beta_stepper=beta;};
        bool GetCurrentPosition(FkPositionBase* position_fk) override {return false;};
        void Calibrate(int step,bool enable_eef_coil);
        void RunM84() override;

        // void SetEffector(EefAction action);

    
    private:
        virtual void IK(FkPositionBase* from_fk, IkPositionBase* to_ik) override;
        virtual void FK(IkPositionBase* from_ik, FkPositionBase* to_fk) override;
        float GetDistanceToTarget_FK() override{return 0.0;};
        float GetDistanceToTarget_IK() override;

        Servo* eefServo;
        bool homed;

        // void SpinOnce_BaseEnter() override {};
        // void SpinOnce_BaseExit() override {};
        void RunG6(Gcode* gcode) override {};   //Block mode
        void RunM123(uint8_t eef_channel, EefAction eef_action) override;
        std::string GetHomeTrigerStateString() override {return " ";};
        // void MoveToTargetPosition() {};
        void _running_G1() override;
        void _running_G28() override;
        void __EnableMotor(char actuator, bool enable_it) override;
    
        // Led objLedPower = Led(0, PIN_LED_POWER_2112, LOW);
        // Led objLedHome_alpha = Led(1,2,LOW);
        // HomeHelper objHomeHelper_alpha = HomeHelper(PIN_HOME_ALPHA, LOW);
        // HomeHelper objHomeHelper_beta = HomeHelper(PIN_HOME_BETA, LOW);
        SingleAxisHomer* alpha_homer;
        SingleAxisHomer* beta_homer;

        // Stepper objStepper_alpha = Stepper(PIN_ALPHA_STEP, PIN_ALPHA_DIR);
        // Stepper objStepper_beta = Stepper(PIN_BETA_STEP, PIN_BETA_DIR);
        Stepper* alpha_stepper;
        Stepper* beta_stepper;
        StepControl objStepControl;

        GobotChessboardHardwareConfig __config;
        Stepper* __homing_stepper;
        SingleAxisHomer* __current_homer;
        FkPosition_XY __current_fk_position;
};


