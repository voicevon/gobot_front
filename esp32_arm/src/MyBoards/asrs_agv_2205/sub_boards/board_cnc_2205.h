#pragma once
// #include "MyBoards/board_base.h"
#include "MyBoards/cnc_board_base.h"
#include "../board_pins/board_pins_ver_2_0.h"
#include "ESP32Step/src/TeensyStep.h"
#include "CNC/single_axis_homer.h"
#include "../robot_eef/garment_asar_eef.h"
#include "CNC/Actuator/stepper/actuator_stepper.h"
// For Core_YZ cnc application
class Board2205Cnc: public CncBoardBase{
    public:
        Board2205Cnc(){};
        void Init(bool is_on_reset) override;
        void Init(Adafruit_MCP23X17* mcp_23018);
        void SayHello() override{};
        ActuatorStepper* GetJointStepper(char axis_name) override;
        SingleAxisHomer* GetHomer(char axis_name) override;
        RobotEef_GarmentAsar* GetEef() override;
        void EnableMotor(char axis_name, bool enable_it) override;
        // void AllMotorsMoveTo(bool is_absolute_position, float* motor_position, int motors_count) override{};
        // void SingleMotorMoveTo(char motor_name, float motor_position) override{};         
        // float GetDistanceToTarget() override{};
        // void AllMotorsMoveTo(bool is_absolute_position, float* motor_position, int motors_count) override{};
        // void AllMotorStop() override {};
        // void SingleMotorMoveTo(char motor_name, float motor_position) override{};         
        // float GetDistanceToTarget() override{return 0;};

        // float GetMotorPosition(char motor_name) override {};
        // void SetMotorPosition(char motor_name, float as_current_position) override {};
        
    private:
        Adafruit_MCP23X17* __mcp23018;
        Stepper stepper_driver_alpha = Stepper(PIN_ALPHA_STEP_2205);
        Stepper stepper_driver_beta = Stepper(PIN_BETA_STEP_2205);
        ActuatorStepper alpha_stepper;
        ActuatorStepper beta_stepper;
        SingleAxisHomer homer_z = SingleAxisHomer(LOW);
        SingleAxisHomer homer_y = SingleAxisHomer(LOW);
        RobotEef_GarmentAsar eef = RobotEef_GarmentAsar();

};

// #endif