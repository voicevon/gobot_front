#pragma once
#include "CNC/board/cnc_board_base.h"
#include "CNC/Actuator/stepper/actuator_stepper.h"
#include "ESP32Step/src/TeensyStep.h"
#include "MyApps/gobot_main/board/board_pins/board_ver1.2.h"
#include "robot_eef/board_spring_maker.h"
#include "Robot/axis_homer/home_trigger_array.h"

class Board_SpringMaker: public CncBoardBase{
    public:
        Board_SpringMaker(){};
        void Init(bool is_on_reset) override;
        
        RobotEef_SpringMaker* GetEef() override;
        void EnableMotor(EnumAxis_Inverseinematic axis_name, bool enable_it) override;
        PositionTrigger* GetPositionTrigger(uint8_t index) override;
        Stepper* GetStepper(){return &this->stepper_driver_alpha;};
        
    private:
        Stepper stepper_driver_alpha = Stepper(PIN_ALPHA_STEP_2201, PIN_ALPHA_DIR_2201);
        // ActuatorStepper stepper_alpha;
        PositionTrigger _all_position_triggers[1];
        // PositionTrigger homer_alpha = PositionTrigger(PIN_HOME_ALPHA_2201, LOW);
        RobotEef_SpringMaker eef = RobotEef_SpringMaker();
};
