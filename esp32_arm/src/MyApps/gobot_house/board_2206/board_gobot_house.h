#pragma once
#include "MyBoards/cnc_board_base.h"
// #include "MyBoards/gobot_house/board_pins/board_pins_ver3.11.h"
#include "../board/board_pins/board_pins_ver3.11.h"
// #include "MyBoards/gobot_house/robot_eef/gobot_house_eef.h"
#include "../board/robot_eef/gobot_house_eef.h"
#include "CNC/Actuator/servo/actuator_servo.h"
#include "CNC/Actuator/stepper/actuator_stepper.h"
#include "CNC/single_axis_homer.h"
#include "cnc_machine.h"

class Board_GobotHouse_2206: public CncBoardBase{
    public:
        Board_GobotHouse_2206(StepControl* stepControl){this->__stepControl=stepControl;};
        void Init(bool is_on_reset) override;
        ActuatorStepper* GetActuator(char axis_name) override;
        ActuatorServo* GetActuaorBeta(char axis_name);
        // MotorBase* GetActuator(char axis_name){};  //TODO: Stepper, Servo, BLDC... all is motorbase!
        SingleAxisHomer* GetHomer(char axis_name) override;
        RobotEef_GobotHouse* GetEef() override;
        CncMachineBase* GetCncMechanic() override {return &this->__cnc_mechanic;};

        void EnableMotor(char axis_name, bool enable_it) override;
        Servo* GetServoDriver_OnBeta(){return &this->__servo_beta;};
        Stepper* GetStepperDriver_OnAlpha(){return &this->__alpha_stepper;};
    
    private:
        void __Init_Hardware();
        
        Stepper __alpha_stepper = Stepper(PIN_ALPHA_STEP_2109, PIN_ALPHA_DIR_2109);
        ActuatorStepper __actuator_alpha;
        ActuatorRangeConstraintBase __alpha_range;
        Servo __servo_beta;
        ActuatorServo __actuator_beta;
        CncMover_StepperServo mover_StepperServo;


        SingleAxisHomer __alpha_homer = SingleAxisHomer(PIN_HOME_ALHPA_2109, LOW);
        SingleAxisHomer __beta_homer = SingleAxisHomer(PIN_HOME_BETA_2109, LOW);

        RobotEef_GobotHouse eef;
        GobotHouseMachine_2206 __cnc_mechanic;

        StepControl* __stepControl;
};
