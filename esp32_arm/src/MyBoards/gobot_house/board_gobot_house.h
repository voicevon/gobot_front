#pragma once
#include "MyBoards/cnc_board_base.h"
#include "board_pins_ver3.11.h"
#include "CNC/single_axis_homer.h"
#include "robot_eef/gobot_house_eef.h"

class Board_GobotHouse: public CncBoardBase{
    public:
        Board_GobotHouse(){};
        void Init(bool is_on_reset) override;
        // void SayHello();
        Stepper* GetStepper(char axis_name) override;
        SingleAxisHomer* GetHomer(char axis_name) override;
        RobotEef_GobotHouse* GetEef() override;
        void EnableMotor(char axis_name, bool enable_it) override;
        
    private:
        Stepper stepper_alpha = Stepper(PIN_ALPHA_STEP_2109, PIN_ALPHA_DIR_2109);
        Stepper stepper_beta = Stepper(PIN_BETA_STEP_2109,PIN_BETA_DIR_2109);
        SingleAxisHomer homer_alpha = SingleAxisHomer(PIN_HOME_ALHPA_2109, LOW);
        SingleAxisHomer homer_beta = SingleAxisHomer(PIN_HOME_BETA_2109, LOW);
        RobotEef_GobotHouse eef = RobotEef_GobotHouse();

};
