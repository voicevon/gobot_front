#pragma once
#include "MyBoards/cnc_board_base.h"
#include "ESP32Step/src/TeensyStep.h"
#include "Robot/homer/single_axis_homer.h"
#include "MyApps/gobot_main/board/board_pins/board_ver1.2.h"
#include "robot_eef/board_spring_maker.h"
#include "CNC/Actuator/stepper/actuator_stepper.h"
// #include "../cnc/solution_config.h"

class Board_SpringMaker: public CncBoardBase{
    public:
        Board_SpringMaker(){};
        void Init(bool is_on_reset) override;
        
        // void SayHello();
        // ActuatorStepper* GetActuator(EnumAxis axis_name) override;
        SingleAxisHomer* GetHomer(EnumAxis axis_name) override;
        RobotEef_SpringMaker* GetEef() override;
        void EnableMotor(EnumAxis axis_name, bool enable_it) override;
        // SpringMaker_CncSolutionConfig* GetCncConfig() override {return &this->__cnc_solution_config;};

        
    private:
        Stepper stepper_driver_alpha = Stepper(PIN_ALPHA_STEP_2201, PIN_ALPHA_DIR_2201);
        ActuatorStepper stepper_alpha;
        SingleAxisHomer homer_alpha = SingleAxisHomer(PIN_HOME_ALPHA_2201, LOW);
        RobotEef_SpringMaker eef = RobotEef_SpringMaker();
        // SpringMaker_CncSolutionConfig __cnc_solution_config;
};
