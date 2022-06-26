#pragma once
// #include "../board_base.h"
// #include "MyBoards/board_base.h"
#include "MyBoards/cnc_board_base.h"

#include "ESP32Step/src/TeensyStep.h"
// #include "CNC/single_axis_homer.h"
#include "mechanic/cnc_solution.h"
// #include "MyBoards/gobot_main/board_pins/board_ver1.2.h"
// #include "MyApps/gobot_main/board/board_pins/board_ver1.2.h"
#include "MyApps/gobot_main/board/board_pins/board_ver1.2.h"
// #include "robot_eef/board_smoke_machine.h"
#include "robot_eef/board_smoke_bot.h"
#include "CNC/Actuator/stepper/actuator_stepper.h"
#include "mechanic/cnc_machine.h"


class Board_SmokeMachine: public CncBoardBase{
    public:
        Board_SmokeMachine(){};
        void Init(bool is_on_reset) override;
        
        void SayHello() override;
        // Stepper* GetStepper(char axis_name) override;
        SingleAxisHomer* GetHomer(EnumAxis axis_name) override;
        RobotEef_SmokeMachine* GetEef() override;
        void EnableMotor(EnumAxis axis_name, bool enable_it) override;
        ActuatorBase* GetActuator(EnumAxis axis_name) override {return &this->__actuator;};
        CncMachineBase* GetCncMechanic() override{return &this->__cnc_mechanic;};

        
    private:
        Stepper stepper_alpha = Stepper(PIN_ALPHA_STEP_2201, PIN_ALPHA_DIR_2201);
        ActuatorStepper __actuator;
        SmookerBot_CncMechanic __cnc_mechanic;
        SingleAxisHomer homer_alpha = SingleAxisHomer(PIN_HOME_ALPHA_2201, LOW);
        RobotEef_SmokeMachine eef = RobotEef_SmokeMachine();


};
