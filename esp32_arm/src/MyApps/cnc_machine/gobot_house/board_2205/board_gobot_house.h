#pragma once
#include "MyLibs/board/board_base.h"
#include "board_pins/board_pins_ver3.11.h"
#include "robot_eef/gobot_house_eef.h"   //TODO: remove
#include "Robot/Sensor/position_binary_trigger/position_trigger_array.h"
#include "CNC/Actuator/stepper/actuator_fast_stepper.h"

class Board_GobotHouse{
    public:
        Board_GobotHouse(){};
        void Init() ;
        // RobotEef_GobotHouse* GetEef() override;
        void EnableMotor(EnumAxis_Inverseinematic actuator_name, bool enable_it) ;
        

    private:
        // Stepper stepper_driver_alpha = Stepper(PIN_ALPHA_STEP_2109, PIN_ALPHA_DIR_2109);
        // Stepper stepper_driver_beta = Stepper(PIN_BETA_STEP_2109,PIN_BETA_DIR_2109);
        FastAccelStepper* alpha_stepper;
        FastAccelStepper* beta_stepper;

        PositionTrigger __all_position_triggers[2];
        // PositionTrigger homer_alpha = PositionTrigger(PIN_HOME_ALHPA_2109, LOW);
        // PositionTrigger homer_beta = PositionTrigger(PIN_HOME_BETA_2109, LOW);
        RobotEef_GobotHouse eef;
        Servo __cnc_beta;

};
