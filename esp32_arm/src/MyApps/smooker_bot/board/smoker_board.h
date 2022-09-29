#pragma once
#include "CNC/board/cnc_board_base.h"
#include "ESP32Step/src/TeensyStep.h"
#include "MyApps/gobot_main/board/board_pins/board_ver1.2.h"
#include "robot_eef/board_smoke_bot.h"
#include "CNC/Actuator/stepper/actuator_stepper.h"


class Smoker_Board: public CncBoardBase{
    public:
        Smoker_Board(){};
        void Init(bool is_on_reset) override;
        
        void SayHello() override;
        RobotEef_SmokeMachine* GetEef() override;
        void EnableMotor(EnumAxis axis_name, bool enable_it) override;

        
    private:
        Stepper stepper_alpha = Stepper(PIN_ALPHA_STEP_2201, PIN_ALPHA_DIR_2201);
        ActuatorStepper __actuator;
        PositionTrigger homer_alpha = PositionTrigger(PIN_HOME_ALPHA_2201, LOW);
        RobotEef_SmokeMachine eef = RobotEef_SmokeMachine();


};
