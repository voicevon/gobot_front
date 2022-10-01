#pragma once
#include "../board_pins/board_pins_ver_2_0.h"
#include "CNC/board/cnc_board_base.h"
#include "ESP32Step/src/TeensyStep.h"
#include "CNC/arm_solution/axis_homer/axis_homer.h"
#include "../robot_eef/garment_asar_eef.h"

// For Core_AZ cnc application
class Board2204Cnc: public CncBoardBase{
    public:
        Board2204Cnc(){};
        void Init(bool is_on_reset) override;
        void Init(Adafruit_MCP23X17* mcp_23018);
        void SayHello() override;
        RobotEef_GarmentAsar* GetEef() override;
        ActuatorBase* GetActuator(EnumAxis_Inverseinematic axis);
        void EnableMotor(EnumAxis_Inverseinematic axis_name, bool enable_it) override;

        
    private:
        Adafruit_MCP23X17* __mcp23018;
        Stepper stepper_alpha = Stepper(PIN_ALPHA_STEP_2205);
        Stepper stepper_beta = Stepper(PIN_BETA_STEP_2205);
        PositionTrigger homer_z = PositionTrigger(LOW);
        PositionTrigger homer_A = PositionTrigger(LOW);
        RobotEef_GarmentAsar eef = RobotEef_GarmentAsar();
        ActuatorStepper __actuator_alpha;
        ActuatorStepper __actuator_beta;

};

// #endif