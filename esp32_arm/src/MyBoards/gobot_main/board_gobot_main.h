#pragma once
// #include "../board_base.h"
#include "MyBoards/cnc_board_base.h"
#include "ESP32Step/src/TeensyStep.h"
#include "CNC/single_axis_homer.h"
#include "board_ver1.2.h"
#include "robot_eef/gobot_main_eef.h"

class Board_GobotMain: public BoardbaseCnc{
    public:
        Board_GobotMain(){};
        void Init(bool is_on_reset) override;
        void SayHello();
        Stepper* GetStepper(char axis_name) override;
        SingleAxisHomer* GetHomer(char axis_name) override;
        RobotEef_GobotMain* GetEef() override;
        void EnableMotor(char axis_name, bool enable_it) override;
        
    private:
        Stepper stepper_alpha = Stepper(PIN_ALPHA_STEP_2201, PIN_ALPHA_DIR_2201);
        Stepper stepper_beta = Stepper(PIN_BETA_STEP_2201, PIN_BETA_DIR_2201);
        SingleAxisHomer homer_alpha = SingleAxisHomer(PIN_HOME_ALPHA_2201, LOW);
        SingleAxisHomer homer_beta = SingleAxisHomer(PIN_HOME_BETA_2201, LOW);
        RobotEef_GobotMain eef = RobotEef_GobotMain();


};
