#pragma once
#include "MyBoards/cnc_board_base.h"
#include "MyBoards/gobot_house/board_pins/board_pins_ver3.11.h"
#include "CNC/single_axis_homer.h"
// #include "robot_eef/gobot_house_eef.h"
#include "MyBoards/gobot_house/robot_eef/gobot_house_eef.h"

class Board_GobotHouse_2206: public CncBoardBase{
    public:
        Board_GobotHouse_2206(){};
        void Init(bool is_on_reset) override;
        // void SayHello();
        // Stepper* GetStepper(char axis_name) override;
        SingleAxisHomer* GetHomer(char axis_name) override;
        RobotEef_GobotHouse* GetEef() override;

        void EnableMotor(char axis_name, bool enable_it) override;

        void AllMotorsMoveTo(bool is_absolute_position, float* motor_position, int motors_count) override{};
        void AllMotorStop() override {};
        void SingleMotorMoveTo(char motor_name, float motor_position) override;         
        float GetDistanceToTarget() override{return 0;};

        float GetMotorPosition(char motor_name) override {};
        void SetMotorPosition(char motor_name, float as_current_position) override {};

    
    private:
        Stepper stepper_alpha = Stepper(PIN_ALPHA_STEP_2109, PIN_ALPHA_DIR_2109);
        // Stepper stepper_beta = Stepper(PIN_BETA_STEP_2109,PIN_BETA_DIR_2109);
        SingleAxisHomer homer_alpha = SingleAxisHomer(PIN_HOME_ALHPA_2109, LOW);
        // SingleAxisHomer homer_beta = SingleAxisHomer(PIN_HOME_BETA_2109, LOW);
        StepControl stepControl;
        RobotEef_GobotHouse eef;

};
