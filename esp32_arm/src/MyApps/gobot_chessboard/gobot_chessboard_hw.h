#pragma once
/*
Stepper liberys:

    https://github.com/gin66/FastAccelStepper
    https://github.com/luni64/TeensyStep
       * Does't support ESP32
    - https://github.com/RCP1/ESP32Step
    https://github.com/Stan-Reifel/FlexyStepper
    - https://github.com/pkerspe/ESP-FlexyStepper
*/

#include <ESP32Servo.h>
#include "Robot/RobotBase.h"
#include "ESP32Step/src/TeensyStep.h"
#include "Robot/Commu/CommuUart.h"
#include "MyLibs/MyFunctions.hpp"
#include "Robot/Gcode.h"

#include "gobot_chessboard_hw_config.h"
#include "Robot/HomeHelper.h"
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
class GobotChessboardHardware: public RobotBase{
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
        void Init() override;
        bool GetCurrentPosition(FkPositionBase* position_fk) override {return false;};
        void Calibrate(int step,bool enable_eef_coil);

        // void SetEffector(EefAction action);

    
    private:
        virtual void IK(FkPositionBase* from_fk, IkPositionBase* to_ik) override;
        virtual void FK(IkPositionBase* from_ik, FkPositionBase* to_fk) override;
        float GetDistanceToTarget_FK() override{return 0.0;};
        float GetDistanceToTarget_IK() override;

        Servo* eefServo;
        bool homed;

    private:
        void SpinOnce_BaseEnter() override {};
        void SpinOnce_BaseExit() override {};
        void RunG6(Gcode* gcode) override {};   //Block mode
        void RunM123(uint8_t eef_channel, EefAction eef_action) override;
        std::string GetHomeTrigerStateString() override {return " ";};
        // void MoveToTargetPosition() {};
        void _running_G1() override;
        void _running_G28() override;
        void __EnableMotor(char actuator, bool enable_it) override;
    
        // Led objLedPower = Led(0, PIN_LED_POWER_2112, LOW);
        // Led objLedHome_alpha = Led(1,2,LOW);
        HomeHelper objHomeHelper_alpha = HomeHelper(PIN_HOME_ALHPA, LOW);
        HomeHelper objHomeHelper_beta = HomeHelper(PIN_HOME_BETA, LOW);

        Stepper objStepper_alpha = Stepper(PIN_ALPHA_STEP, PIN_ALPHA_DIR);
        Stepper objStepper_beta = Stepper(PIN_BETA_STEP, PIN_BETA_DIR);
        StepControl objStepControl;

        GobotChessboardHardwareConfig __config;
        Stepper* __homing_stepper;
        HomeHelper* __homing_helper;
        FkPosition_XY __current_fk_position;
};

