#pragma once

#include "cnc_mover_base.h"
#include "ESP32Step/src/TeensyStep.h"
#include "CNC/Actuator/servo/actuator_servo.h"
#include "CNC/Actuator/stepper/actuator_stepper.h"

class CncMover_StepperServo: public CncMoverBase{
    public:
        void LinkStepControl(StepControl* stepControl){this->__stepControl=stepControl;};
        void LinkStepper_asAlpha(ActuatorStepper* stepper);
        void LinkServo_asBeta(ActuatorServo* servo);
        void PrintOut(const char* title) override;
        
        void SetEefSpeed(float speed) override;
        // float GetSingleActuatorCurrentPosition_InCncUnit(EnumAxis motor_name) override;
        // float GetAbsDistanceToTarget_InCncUnit() override;
        // void SetActuatorSpeed(EnumAxis actuator_name, float rad_per_second) override;
        bool ActuatorIsMoving(EnumAxis actuator) override;


        void AllActuatorsMoveTo(uint8_t is_absolute_position, float* positions_in_cnc_unit) override;
        void SingleActuatorMoveTo(EnumAxis actuator_name, bool is_absolute_position, float position_in_cnc_unit) override;
        void AllActuatorsStop() override;
        void SingleActuatorStop(EnumAxis motor_name) override;

        // void SetActuatorCurrentCncPositionAs(EnumAxis actuator_name, float as_current_position) override;
        // void Stop(char motor_name) override;
        // struct MotorFlag{

        // }
        
    protected:

    private:
        // Stepper* __stepper_alpha;
        ActuatorStepper* __actuator_alpha;
        ActuatorServo* __actuator_beta;
        StepControl* __stepControl;
};