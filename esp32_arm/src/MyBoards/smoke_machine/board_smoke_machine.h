#pragma once
#include "../board_base.h"
#include "MyBoards/cnc_board_base.h"

#include "ESP32Step/src/TeensyStep.h"
#include "CNC/single_axis_homer.h"
#include "MyBoards/gobot_main/board_pins/board_ver1.2.h"
#include "robot_eef/board_smoke_machine.h"

class Board_SmokeMachine: public CncBoardBase{
    public:
        Board_SmokeMachine(){};
        void Init(bool is_on_reset) override;
        
        void SayHello();
        // Stepper* GetStepper(char axis_name) override;
        SingleAxisHomer* GetHomer(char axis_name) override;
        RobotEef_SmokeMachine* GetEef() override;
        void EnableMotor(char axis_name, bool enable_it) override;

        
    private:
        Stepper stepper_alpha = Stepper(PIN_ALPHA_STEP_2201, PIN_ALPHA_DIR_2201);
        SingleAxisHomer homer_alpha = SingleAxisHomer(PIN_HOME_ALPHA_2201, LOW);
        RobotEef_SmokeMachine eef = RobotEef_SmokeMachine();


};
