#pragma once
#include "MyBoards/cnc_board_base.h"
#include "MyBoards/gobot_house/board_pins/board_pins_ver3.11.h"
#include "CNC/single_axis_homer.h"
#include "MyBoards/gobot_house/robot_eef/gobot_house_eef.h"
#include "CNC/Actuator/servo/actuator_servo.h"
#include "CNC/Actuator/stepper/actuator_stepper.h"

class Board_GobotHouse_2206: public CncBoardBase{
    public:
        Board_GobotHouse_2206(){};
        void Init(bool is_on_reset) override;
        // void SayHello();
        ActuatorStepper* GetJointStepper(char axis_name) override;
        ActuatorServo* GetJointServo(char axis_name);
        // MotorBase* GetMotor(char axis_name){};  //TODO: Stepper, Servo, BLDC... all is motorbase!
        SingleAxisHomer* GetHomer(char axis_name) override;
        RobotEef_GobotHouse* GetEef() override;
        // CncMover_StepperServo* GetCncMover();

        void EnableMotor(char axis_name, bool enable_it) override;
        Servo* GetServoDriver_OnBeta(){return &this->__beta_servo_driver;};

    
    private:
        Stepper __alpha_stepper_driver = Stepper(PIN_ALPHA_STEP_2109, PIN_ALPHA_DIR_2109);
        ActuatorStepper __alpha_stepper;
        SingleAxisHomer __alpha_homer = SingleAxisHomer(PIN_HOME_ALHPA_2109, LOW);

        SingleAxisHomer __beta_homer = SingleAxisHomer(PIN_HOME_BETA_2109, LOW);
        Servo __beta_servo_driver;
        ActuatorServo __beta_servo;
        CncMover_StepperServo mover_StepperServo;

        RobotEef_GobotHouse eef;

};
