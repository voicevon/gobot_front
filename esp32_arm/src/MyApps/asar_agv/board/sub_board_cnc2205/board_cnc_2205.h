#pragma once
#include "MyBoards/board_base.h"
// #include "CNC/board/cnc_board_base.h"
#include "../board_pins/board_pins_ver_2_0.h"
#include "FastAccelStepper.h"
// #include "ESP32Step/src/TeensyStep.h"
#include "Robot/axis_homer/position_trigger_array.h"
#include "../robot_eef/garment_asar_eef.h"
// #include "CNC/Actuator/stepper/actuator_stepper.h"
#include "CNC/Actuator/stepper/actuator_fast_stepper.h"



// For Core_YZ cnc application
class Board2205Cnc{
    public:
        Board2205Cnc(){};
        void Init(bool is_on_reset) ;
        void Init(Adafruit_MCP23X17* mcp_23018);
        // void SayHello() override{};
        // RobotEef_GarmentAsar* GetEef() override;
        void EnableMotor(EnumAxis_Inverseinematic axis_name, bool enable_it) ;
        // PositionTrigger* GetPositionTrigger(uint8_t index) override{return nullptr;};
        CncActuatorBase* GetActuator(EnumAxis_Inverseinematic axis){return nullptr;};

        
    private:
        Adafruit_MCP23X17* __mcp23018;
        // Stepper stepper_driver_alpha = Stepper(PIN_ALPHA_STEP_2205);
        // Stepper stepper_driver_beta = Stepper(PIN_BETA_STEP_2205);
        FastAccelStepper stepper_driver_alpha; // = Stepper(PIN_ALPHA_STEP_2205);
        FastAccelStepper stepper_driver_beta; // = Stepper(PIN_BETA_STEP_2205);
         
        PositionTrigger __all_position_triggers[2];
        // PositionTrigger homer_z = PositionTrigger(LOW);
        // PositionTrigger homer_y = PositionTrigger(LOW);
        RobotEef_GarmentAsar eef = RobotEef_GarmentAsar();

};

// #endif