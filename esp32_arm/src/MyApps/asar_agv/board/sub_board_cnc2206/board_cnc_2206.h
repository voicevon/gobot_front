#pragma once
#include "MyLibs/board/board_base.h"
#include "FastAccelStepper.h"
#include "Robot/axis_homer/position_trigger_array.h"
#include "CNC/Actuator/stepper/actuator_fast_stepper.h"
#include "../board_pins/board_pins_ver_2_0.h"
#include "../robot_eef/garment_asar_eef.h"   //todo remove


// For Core_AZ cnc application
class Board2204Cnc:public BoardBase{
    public:
        Board2204Cnc();
        void Init() ;
        void Init(Adafruit_MCP23X17* mcp_23018);
        void GetStepper(EnumAxis_Inverseinematic axis);
        void EnableMotor(EnumAxis_Inverseinematic axis_name, bool enable_it) ;

        
    private:
        Adafruit_MCP23X17* __mcp23018;
        FastAccelStepper* stepper_alpha; // = Stepper(PIN_ALPHA_STEP_2205);
        FastAccelStepper* stepper_beta;  // = Stepper(PIN_BETA_STEP_2205);
        PositionTrigger __all_position_triggers[2];
        // PositionTrigger homer_z = PositionTrigger(LOW);
        // PositionTrigger homer_A = PositionTrigger(LOW);
        RobotEef_GarmentAsar eef = RobotEef_GarmentAsar();
        // CncActuatorStepper __actuator_alpha;
        // FastAccelStepper __actuator_beta;

};

// #endif