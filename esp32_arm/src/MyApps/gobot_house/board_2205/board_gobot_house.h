#pragma once
#include "CNC/board/cnc_board_base.h"
// #include "MyBoards/cnc_board_base.h"
#include "board_pins/board_pins_ver3.11.h"
#include "robot_eef/gobot_house_eef.h"
// #include "CNC/Sensor/position_trigger/position_triggers.h"
#include "CNC/solution/axis_homer/axis_homer.h"
#include "CNC/Actuator/stepper/actuator_stepper.h"
// #include "CNC/mover/dual_stepper.h"

class Board_GobotHouse: public CncBoardBase{
    public:
        Board_GobotHouse(){};
        void Init(bool is_on_reset) override;
        // void SayHello();
        // ActuatorBase* GetActuator(EnumAxis axis_name) override;
        // SinglePositionTrigger* GetSingleHomer(EnumAxis axis_name) override;
        // AxisHomer* GetPositionTriggers(EnumAxis axis_name) override;
        RobotEef_GobotHouse* GetEef() override;
        void EnableMotor(EnumAxis actuator_name, bool enable_it) override;
        // KinematicConfig* GetCncConfig() override {return &this->__cnc_mechanic;};
        

    private:
        Stepper stepper_driver_alpha = Stepper(PIN_ALPHA_STEP_2109, PIN_ALPHA_DIR_2109);
        Stepper stepper_driver_beta = Stepper(PIN_BETA_STEP_2109,PIN_BETA_DIR_2109);
        ActuatorStepper alpha_stepper;
        ActuatorStepper beta_stepper;
        // CncMover_DualStepper mover_StepperServo;
        // GobotHouse_CncSolotuionConfig_2205 __cnc_mechanic;

        SinglePositionTrigger homer_alpha = SinglePositionTrigger(PIN_HOME_ALHPA_2109, LOW);
        SinglePositionTrigger homer_beta = SinglePositionTrigger(PIN_HOME_BETA_2109, LOW);
        RobotEef_GobotHouse eef;
        Servo __cnc_beta;

};
