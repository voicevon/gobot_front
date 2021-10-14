#pragma once
#include "Robot/RobotBase.h"

#include "ESP32Step/src/TeensyStep.h"
#include "Robot/HomeHelper.h"
#include "Robot/Commu/CommuUart.h"


#define PIN_HOME 22
#define PIN_STEP 23
#define PIN_DIR 24


class SmokeBot: RobotBase{
    public:
        SmokeBot();


    private:
        CommuUart objCommuUart = CommuUart();
        SingleAxis objAxis_Alpha= SingleAxis();
        Stepper objStepper = Stepper(PIN_STEP, PIN_DIR);
        StepControl objStepControl;
        HomeHelper objHomeHelper = HomeHelper(PIN_HOME, HIGH);
        
        void Init_Linkage() override;
        // void HomeAllAxises() {}override;
        // void MoveToTargetPosition() {} override;
        // void SpinOnce_BaseEnter() {}override;
        // void SpinOnce_BaseExit() override;
        // IkPositionBase* IK(FkPositionBase* fk) override;
        // FkPositionBase* FK(IkPositionBase* ik) override;
        // void RunG1(Gcode* gcode) override;   //None blocking, move backgroundly.
        // void RunG6(Gcode* gcode) override;   //Block mode
        // std::string GetHomeTrigerStateString() override;

};