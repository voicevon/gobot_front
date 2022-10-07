#pragma once
#include "CNC/board/cnc_board_base.h"
#include "ESP32Step/src/TeensyStep.h"
#include "board_pins/board_ver1.2.h"
#include "robot_eef/gobot_main_eef.h"
// #include "Robot/axis_homer/axis_homer.h"

class GobotMain_Board: public CncBoardBase{
    public:
        GobotMain_Board(){};
        void Init(bool is_on_reset) override;
        Stepper* GetStepper(EnumAxis_Inverseinematic axis);
        RobotEef_GobotMain* GetEef() override;
        void EnableMotor(EnumAxis_Inverseinematic axis_name, bool enable_it) override;
        uint8_t ReadAllRooms();
        uint8_t GetLoadedRoom();
        CncActuatorStepper* GetActuator(EnumAxis_Inverseinematic axis){return nullptr;};

    private:
        void InitHardware();
        Stepper __alpha_stepper = Stepper(PIN_ALPHA_STEP_2201, PIN_ALPHA_DIR_2201);
        Stepper __beta_stepper = Stepper(PIN_BETA_STEP_2201, PIN_BETA_DIR_2201);
        PositionTrigger __all_position_triggers[2];
        RobotEef_GobotMain __eef;

        uint8_t PIN_ROOMS[8];
 

};
