#pragma once
#include "../board_base.h"
#include "ESP32Step/src/TeensyStep.h"
// #include "ESP32Step/src/Stepper.h"
#include "CNC/single_axis_homer.h"
// #include "gobot_house_ver3.11.h"
#include "board_gobot_house_ver3.11.h"

class BoardPart_Cnc: public BoardbaseCnc{
    public:
        BoardPart_Cnc(){};
        void Init() override;
        // void Init(Adafruit_MCP23X17* mcp_23018);
        void SayHello();
        // void EnableMotor_alpha(bool enable_it) override;
        // void EnableMotor_beta(bool enable_it) override;
        Stepper* GetStepper(char axis_name) override;
        SingleAxisHomer* GetHomer(char axis_name) override;
        void EnableMotor(char axis_name, bool enable_it) override;
        
    private:
        // Adafruit_MCP23X17* __mcp23018;
        // Stepper stepper_alpha = Stepper(PIN_ALPHA_STEP_2109, PIN_ALPHA_DIR_2109);
        // Stepper stepper_beta = Stepper(PIN_BETA_STEP_2109,PIN_BETA_DIR_2109);
        SingleAxisHomer homer_z = SingleAxisHomer(LOW);
        SingleAxisHomer homer_y = SingleAxisHomer(LOW);

};
