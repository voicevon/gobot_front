#pragma once
#include "MyBoards/cnc_board_base.h"
#include "MyBoards/gobot_house/board_pins/board_pins_ver3.11.h"
#include "CNC/single_axis_homer.h"
#include "MyBoards/gobot_house/robot_eef/gobot_house_eef.h"

class Board_GobotHouse_2206: public CncBoardBase{
    public:
        Board_GobotHouse_2206(){};
        void Init(bool is_on_reset) override;
        // void SayHello();
        Stepper* GetJointStepper(char axis_name) override;
        Servo* GetJointServo(char axis_name);
        // MotorBase* GetMotor(char axis_name){};  //TODO: Stepper, Servo, BLDC... all is motorbase!
        SingleAxisHomer* GetHomer(char axis_name) override;
        RobotEef_GobotHouse* GetEef() override;
        // CncMover_StepperServo* GetCncMover();

        void EnableMotor(char axis_name, bool enable_it) override;

    
    private:
        Stepper __alpha_stepper = Stepper(PIN_ALPHA_STEP_2109, PIN_ALPHA_DIR_2109);
        SingleAxisHomer __alpha_homer = SingleAxisHomer(PIN_HOME_ALHPA_2109, LOW);
        Servo __beta_servo;
        CncMover_StepperServo mover_StepperServo;

        // StepControl stepControl;
        RobotEef_GobotHouse eef;

};
