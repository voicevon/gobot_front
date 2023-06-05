#pragma once
#include "board_pins/board_ver1.2.h"

#include "FastAccelStepper.h"

#include "Robot/board/board_base.h"
#include "CNC/Actuator/stepper/actuator_fast_stepper.h"
#include "Robot/Sensor/position_binary_trigger/position_trigger_array.h"
#include "robot_eef/gobot_main_eef.h"


class GobotMain_Board: public BoardBase{
    public:
        GobotMain_Board(){};
        void Init(const char* app_welcome_statement) override;
        // Stepper* GetStepper(EnumAxis_Inverseinematic axis);
        FastAccelStepper* GetStepper(EnumAxis_Inverseinematic axis);
        void EnableMotor(EnumAxis_Inverseinematic axis_name, bool enable_it);
        uint8_t ReadAllRooms();
        uint8_t GetLoadedRoom();
        // CncActuatorStepper* GetActuator(EnumAxis_Inverseinematic axis){return nullptr;};
        CncActuatorFastStepper* GetActuator(EnumAxis_Inverseinematic axis){return nullptr;};

    private:
        void InitHardware();
        // Stepper __alpha_stepper = Stepper(PIN_ALPHA_STEP_2201, PIN_ALPHA_DIR_2201);
        // Stepper __beta_stepper = Stepper(PIN_BETA_STEP_2201, PIN_BETA_DIR_2201);
        FastAccelStepper __alpha_stepper;
        FastAccelStepper __beta_stepper;
        PositionTrigger __all_position_triggers[2];
        RobotEef_GobotMain __eef;

        uint8_t PIN_ROOMS[8];
 

};
