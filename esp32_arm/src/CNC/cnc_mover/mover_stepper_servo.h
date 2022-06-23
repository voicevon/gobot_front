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
        
        void SetSpeed(float speed) override;
        float GetSingleActuatorCurrentPosition_InCncUnit(char motor_name) override;
        float GetAbsDistanceToTarget_InCncUnit() override;
        void SetActuatorSpeed(char actuator_name, float rad_per_second) override;
        bool ActuatorIsMoving(char actuator) override;


        void AllActuatorsMoveTo(uint8_t is_absolute_position, float* positions_in_cnc_unit) override;
        void SingleActuatorMoveTo(char actuator_name, bool is_absolute_position, float position_in_cnc_unit) override;
        void AllActuatorsStop() override;
        void SingleActuatorStop(char motor_name) override;

        void SetActuatorCurrentCncPositionAs(char actuator_name, float as_current_position) override;
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