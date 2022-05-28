#pragma once
#include "board_pins_ver_2_0.h"
#include "MyLibs/board_base.h"
// #include "ESP32Step/src/TeensyStep.h"
#include "ESP32Step/src/Stepper.h"
#include "CNC/single_axis_homer.h"

class BoardPart_Cnc: public BoardbaseCnc{
    public:
        BoardPart_Cnc(){};
        void Init(Adafruit_MCP23X17* mcp_23018);
        void SayHello();
        void EnableMotor_alpha(bool enable_it) override;
        void EnableMotor_beta(bool enable_it) override;
        Stepper stepper_alpha = Stepper(PIN_ALPHA_STEP);
        Stepper stepper_beta = Stepper(PIN_BETA_STEP);
        SingleAxisHomer homer_z = SingleAxisHomer(LOW);
        SingleAxisHomer homer_y = SingleAxisHomer(LOW);
        
    private:
        Adafruit_MCP23X17* __mcp23018;


};