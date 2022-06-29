#pragma once
#include "../board_pins/board_pins_ver_2_0.h"
#include "MyBoards/cnc_board_base.h"
#include "ESP32Step/src/TeensyStep.h"
// #include "CNC/single_axis_homer.h"
#include "Robot/homer/single_axis_homer.h"
#include "../robot_eef/garment_asar_eef.h"

// For Core_AZ cnc application
class Board2204Cnc: public CncBoardBase{
    public:
        Board2204Cnc(){};
        void Init(bool is_on_reset) override;
        void Init(Adafruit_MCP23X17* mcp_23018);
        void SayHello() override;
        // Stepper* GetStepper(char axis_name) override;
        SingleAxisHomer* GetHomer(EnumAxis axis_name) override;
        RobotEef_GarmentAsar* GetEef() override;
        void EnableMotor(EnumAxis axis_name, bool enable_it) override;

        
    private:
        Adafruit_MCP23X17* __mcp23018;
        Stepper stepper_alpha = Stepper(PIN_ALPHA_STEP_2205);
        Stepper stepper_beta = Stepper(PIN_BETA_STEP_2205);
        SingleAxisHomer homer_z = SingleAxisHomer(LOW);
        SingleAxisHomer homer_A = SingleAxisHomer(LOW);
        RobotEef_GarmentAsar eef = RobotEef_GarmentAsar();

};

// #endif