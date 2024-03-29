#pragma once
#include "Robot/board/board_base.h"
#include "CNC/Actuator/servo/cnc_actuator_servo.h"
#include "CNC/Actuator/stepper/actuator_fast_stepper.h"
#include "Robot/Sensor/position_binary_trigger/position_trigger_array.h"
#include "../board_2205/board_pins/board_pins_ver3.11.h"
#include "../board_2205/robot_eef/gobot_house_eef.h"

class Board_GobotHouse_2206{
    public:
        // Board_GobotHouse_2206(StepControl* stepControl){this->__stepControl=stepControl;};
        Board_GobotHouse_2206(){};
        void Init();
        // CncActuatorBase* GetActuator(EnumAxis axis_name) override;
        CncActuatorServo* GetActuaorBeta(char axis_name);
        // MotorBase* GetActuator(char axis_name){};  //TODO: Stepper, Servo, BLDC... all is motorbase!
        // PositionTrigger* GetSingleHomer(EnumAxis axis_name) override;
        // AxisHomer* GetPositionTriggers(EnumAxis axis_name) override;

        // RobotEef_GobotHouse* GetEef() override;

        void EnableMotor(EnumAxis_Inverseinematic axis_name, bool enable_it);
        Servo* GetServoDriver_OnBeta(){return &this->__servo_beta;};
        // Stepper* GetStepperDriver_OnAlpha(){return &this->__alpha_stepper;};
        FastAccelStepper* GetStepperDriver_OnAlpha(){return &this->__alpha_stepper;};
    
    private:
        void __Init_Hardware();
        
        // Stepper __alpha_stepper = Stepper(PIN_ALPHA_STEP_2109, PIN_ALPHA_DIR_2109);
        FastAccelStepper __alpha_stepper;
        // CncActuatorStepper __actuator_alpha;
        CncActuatorFastStepper __actuator_alpha;
        ActuatorRangeConstraintBase __alpha_range;
        Servo __servo_beta;
        CncActuatorServo __actuator_beta;
        // CncMover_StepperServo mover_StepperServo;

        // PositionTrigger* GetPositionTrigger(uint8_t index) override{return nullptr;};

        PositionTrigger __all_position_triggers[2];
        RobotEef_GobotHouse eef;
        // GobotHouse_CncSolutionConfig_2206 __cnc_mechanic;

        // StepControl* __stepControl;
};
