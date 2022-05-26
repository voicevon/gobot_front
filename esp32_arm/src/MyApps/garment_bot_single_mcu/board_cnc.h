#pragma once
#include "board_pins_ver_2_0.h"
#include "MyLibs/board_base.h"
// #include "ESP32Step/src/TeensyStep.h"
#include "ESP32Step/src/Stepper.h"

class BoardPart_Cnc: public BoardbaseCnc{
    public:
        BoardPart_Cnc();
        void Init(Adafruit_MCP23X17* mcp_23018){this->__mcp23018=mcp_23018;};
        void EnableMotor_alpha(bool enable_it) override;
        void EnableMotor_beta(bool enable_it) override;
        Stepper stepper_alpha = Stepper(PIN_ALPHA_STEP, this->__mcp23018, MC23018_PIN_ALPHA_DIR);
        Stepper stepper_beta = Stepper(PIN_BETA_STEP, this->__mcp23018, MC23018_PIN_BETA_DIR);
        
    private:
        Adafruit_MCP23X17* __mcp23018;


};