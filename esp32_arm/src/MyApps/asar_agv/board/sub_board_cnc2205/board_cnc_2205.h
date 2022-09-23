#pragma once
#include "MyBoards/cnc_board_base.h"
#include "../board_pins/board_pins_ver_2_0.h"
#include "ESP32Step/src/TeensyStep.h"
// #include "CNC/Sensor/position_trigger/position_triggers.h"
#include "CNC/solution/axis_homer/axis_homer.h"
#include "../robot_eef/garment_asar_eef.h"


// For Core_YZ cnc application
class Board2205Cnc: public CncBoardBase{
    public:
        Board2205Cnc(){};
        void Init(bool is_on_reset) override;
        void Init(Adafruit_MCP23X17* mcp_23018);
        void SayHello() override{};
        // ActuatorBase* GetActuator(EnumAxis axis_name) override;
        SinglePositionTrigger* GetSingleHomer(EnumAxis axis_name) override;
        AxisHomer* GetPositionTriggers(EnumAxis axis_name) override;
        RobotEef_GarmentAsar* GetEef() override;
        void EnableMotor(EnumAxis axis_name, bool enable_it) override;
        // CncSolutionConfigBase* GetCncConfig() override {return nullptr;};

        
    private:
        Adafruit_MCP23X17* __mcp23018;
        Stepper stepper_driver_alpha = Stepper(PIN_ALPHA_STEP_2205);
        Stepper stepper_driver_beta = Stepper(PIN_BETA_STEP_2205);
        // ActuatorStepper alpha_stepper;
        // ActuatorStepper beta_stepper;
        SinglePositionTrigger homer_z = SinglePositionTrigger(LOW);
        SinglePositionTrigger homer_y = SinglePositionTrigger(LOW);
        RobotEef_GarmentAsar eef = RobotEef_GarmentAsar();

};

// #endif