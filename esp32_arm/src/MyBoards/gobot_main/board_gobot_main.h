#pragma once
#include "../board_base.h"
#include "ESP32Step/src/TeensyStep.h"
// #include "ESP32Step/src/Stepper.h"
#include "CNC/single_axis_homer.h"
// #include "gobot_house_ver3.11.h"
// #include "board_gobot_house_ver3.11.h"
#include "board_ver1.2.h"
#include "CNC/RobotEef/gobot_main/gobot_main.h"

class Board_GobotMain: public BoardbaseCnc{
    public:
        Board_GobotMain(){};
        void Init() override;
        void SayHello();
        Stepper* GetStepper(char axis_name) override;
        SingleAxisHomer* GetHomer(char axis_name) override;
        RobotEefBase* GetEef() override;
        void EnableMotor(char axis_name, bool enable_it) override;
        
    private:
        Stepper stepper_alpha = Stepper(PIN_ALPHA_STEP_2201, PIN_ALPHA_DIR_2201);
        Stepper stepper_beta = Stepper(PIN_BETA_STEP_2201, PIN_BETA_DIR_2201);
        SingleAxisHomer homer_alpha = SingleAxisHomer(PIN_HOME_ALPHA_2201, LOW);
        SingleAxisHomer homer_beta = SingleAxisHomer(PIN_HOME_BETA_2201, LOW);
        RobotEef_GobotMain eef = RobotEef_GobotMain();


};
