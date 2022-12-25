#pragma once
// #include "CNC/board/cnc_board_base.h"
// #include "Robot/axis_homer/axis_homer.h"
// #include "CNC/arm_solution/axis_homer/axis_homer.h"
#include "MyBoards/board_base.h"
#include "MyApps/gobot_main/board/board_pins/board_ver1.2.h"
#include "robot_eef/board_smoke_bot.h"    // todo: remove
#include "CNC/Actuator/stepper/actuator_stepper.h"
#include "FastAccelStepper.h"
// #include "ESP32Step/src/TeensyStep.h"
#include "Robot/axis_homer/position_trigger_array.h"


class Smoker_Board: public BoardBase{
    public:
        Smoker_Board(){};
        void Init(bool is_on_reset) override;
        
        // void SayHello() override;
        // RobotEef_SmokeMachine* GetEef() override;
        void EnableMotor(EnumAxis_Inverseinematic axis_name, bool enable_it) ;
        CncActuatorBase* GetActuator(EnumAxis_Inverseinematic axis) {return nullptr;};   //todo:  get it.
        // PositionTrigger* GetPositionTrigger(uint8_t index) override {return nullptr;}; 

    private:
        Stepper stepper_alpha = Stepper(PIN_ALPHA_STEP_2201, PIN_ALPHA_DIR_2201);
        CncActuatorStepper __actuator;
        PositionTrigger __all_position_trigger[1];
        RobotEef_SmokeMachine eef = RobotEef_SmokeMachine();


};
