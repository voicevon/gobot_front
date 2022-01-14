#pragma once

#include "Robot/RobotBase.h"

#include "ESP32Step/src/TeensyStep.h"
#include "Robot/Actuator/ActuatorBase.h"
#include "Robot/HomeHelper.h"
#include "Robot/Commu/CommuUart.h"

#define PIN_HOME 18
#define PIN_STEP 33
#define PIN_DIR 32
#define PIN_ENABLE 14
#define MICRO_STEP_1 27
#define MICRO_STEP_2 26
#define MICRO_STEP_3 25



class SmokeBot: public RobotBase{
    public:
        SmokeBot();
        void HomeSingleAxis(char axis) override;
        void Init_Linkage() override;
        void Init_Gpio();


    private:
        CommuUart objCommuUart = CommuUart();
        // SingleAxis objAxis_Alpha= SingleAxis();
        ActuatorBase objActuator = ActuatorBase();
        Stepper objStepper = Stepper(PIN_STEP, PIN_DIR);
        StepControl objStepControl;

        HomeHelper objHomeHelper = HomeHelper(PIN_HOME, HIGH);
        
        void MoveToTargetPosition()  override {};
        void SpinOnce_BaseEnter() override {};
        void SpinOnce_BaseExit() override {};
        IkPositionBase* IK(FkPositionBase* fk) override {};
        FkPositionBase* FK(IkPositionBase* ik) override {};
        void RunG1(Gcode* gcode) override;   //None blocking, move backgroundly.
        void RunG6(Gcode* gcode) override {};   //Block mode
        std::string GetHomeTrigerStateString() override {};

        void _running_G1() override;
        void _running_G28() override;


};