#pragma once
#include "MyBoards/board_base.h"
#include "board_pins_ver_2_0.h"
#include "ESP32Step/src/TeensyStep.h"
#include "CNC/single_axis_homer.h"
#include "CNC/RobotEef/garment_asrs_agv/garment_asar.h"


class CncDriver_CoreYZ_2205: public BoardbaseCnc{
    public:
        CncDriver_CoreYZ_2205(){};
        void Init() override;
        void Init(Adafruit_MCP23X17* mcp_23018);
        void SayHello();
        Stepper* GetStepper(char axis_name) override;
        SingleAxisHomer* GetHomer(char axis_name) override;
        RobotEef_GarmentAsar* GetEef() override;
        void EnableMotor(char axis_name, bool enable_it) override;

        
    private:
        Adafruit_MCP23X17* __mcp23018;
        Stepper stepper_alpha = Stepper(PIN_ALPHA_STEP_2205);
        Stepper stepper_beta = Stepper(PIN_BETA_STEP_2205);
        SingleAxisHomer homer_z = SingleAxisHomer(LOW);
        SingleAxisHomer homer_y = SingleAxisHomer(LOW);
        RobotEef_GarmentAsar eef = RobotEef_GarmentAsar();

};

// #endif