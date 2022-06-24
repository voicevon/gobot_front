#pragma once
// #include "../board_base.h"
#include "MyBoards/cnc_board_base.h"

#include "ESP32Step/src/TeensyStep.h"
#include "CNC/single_axis_homer.h"
// #include "MyBoards/gobot_main/board_pins/board_ver1.2.h"
#include "MyApps/gobot_main/board/board_pins/board_ver1.2.h"
#include "robot_eef/board_spring_maker.h"
#include "CNC/Actuator/stepper/actuator_stepper.h"
#include "cnc_machine.h"

class Board_SpringMaker: public CncBoardBase{
    public:
        Board_SpringMaker(){};
        void Init(bool is_on_reset) override;
        
        // void SayHello();
        ActuatorStepper* GetActuator(char axis_name) override;
        SingleAxisHomer* GetHomer(char axis_name) override;
        RobotEef_SpringMaker* GetEef() override;
        void EnableMotor(char axis_name, bool enable_it) override;
        CncMachineBase* GetCncMechanic() override {return &this->__cnc_mechanic;};

        
    private:
        Stepper stepper_driver_alpha = Stepper(PIN_ALPHA_STEP_2201, PIN_ALPHA_DIR_2201);
        ActuatorStepper stepper_alpha;
        SingleAxisHomer homer_alpha = SingleAxisHomer(PIN_HOME_ALPHA_2201, LOW);
        RobotEef_SpringMaker eef = RobotEef_SpringMaker();
        SpringMakerMachine __cnc_mechanic;
};
