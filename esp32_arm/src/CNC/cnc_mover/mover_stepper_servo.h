#pragma once

#include "cnc_mover_base.h"
#include "ESP32Step/src/TeensyStep.h"
#include "CNC/Actuator/servo/actuator_servo.h"
#include "CNC/Actuator/stepper/actuator_stepper.h"

class CncMover_StepperServo: public CncMoverBase{
    public:
        // void LinkStepper_asAlpha(Stepper* stepper);
        void LinkStepper_asAlpha(ActuatorStepper* stepper);
        void LinkServo_asBeta(ActuatorServo* servo);
        void AllMotorsMoveTo(bool is_absolute_position, float* positions_in_cnc_unit, int motors_count) override;
        void AllMotorStop() override;
        void SingleMotorMoveTo(bool is_absolute_position, char motor_name, float position_in_cnc_unit) override;
        void SingleMotorStop(char motor_name) override;

        float GetMotorPosition_InCncUnit(char motor_name) override;
        void SetMotorPosition(char motor_name, float as_current_position) override; 
        float GetDistanceToTarget_InCncUnit() override;
        void SetActuatorSpeed(char actuator_name, float speed_per_second) override;
        bool MotorIsMoving(char moto_name) override;
        // void Stop(char motor_name) override;

        
    protected:

    private:
        // Stepper* __stepper_alpha;
        ActuatorStepper* __stepper_alpha;
        ActuatorServo* __servo_beta;
        StepControl __stepControl;
};