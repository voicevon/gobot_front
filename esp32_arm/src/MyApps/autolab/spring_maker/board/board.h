#pragma once
#include "Robot/board/board_base.h"
#include "CNC/Actuator/stepper/actuator_fast_stepper.h"
#include "Robot/Sensor/position_binary_trigger/position_trigger_array.h"
// #include "MyApps/cnc_machine/gobot_main/board/board_pins/board_ver1.2.h"
#include "MyApps/gobot/gobot_main/board/board_pins/board_ver1.2.h"
#include "FastAccelStepper.h"
#include "robot_eef/board_spring_maker.h"


#define POSITION_TRIGGERS_COUNT 1

class Board_SpringMaker: public BoardBase{
    public:
        Board_SpringMaker(){};
        void Init(const char* app_welcome_statement) override;
        
        void EnableMotor(EnumAxis_Inverseinematic axis_name, bool enable_it);
        FastAccelStepper* GetStepper(){return &this->stepper_driver_alpha;};
        
    private:
        // Stepper stepper_driver_alpha = Stepper(PIN_ALPHA_STEP_2201, PIN_ALPHA_DIR_2201);
        FastAccelStepper stepper_driver_alpha;
        PositionTrigger _all_position_triggers[POSITION_TRIGGERS_COUNT];
        // PositionTrigger homer_alpha = PositionTrigger(PIN_HOME_ALPHA_2201, LOW);
        RobotEef_SpringMaker eef = RobotEef_SpringMaker();
};
