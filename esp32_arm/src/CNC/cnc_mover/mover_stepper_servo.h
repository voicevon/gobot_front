#pragma once

#include "cnc_mover_base.h"
#include "ESP32Step/src/Stepper.h"
#include "ESP32Step/src/TeensyStep.h"
#include <ESP32Servo.h>


class CncMover_StepperServo: public CncMoverBase{
    public:
        void LinkStepper_asAlpha(Stepper* stepper);
        void LinkServo_asBeta(Servo* servo);
        void AllMotorsMoveTo(bool is_absolute_position, float* motor_position, int motors_count);
        void AllMotorStop();
        void SingleMotorMoveTo(bool is_absolute_position, char motor_name, float motor_position);
        float GetMotorPosition(char motor_name);
        void SetMotorPosition(char motor_name, float as_current_position);
        float GetDistanceToTarget();
        
    protected:

    private:
        Stepper* __stepper_alpha;
        Servo* __servo_beta;
        StepControl __stepControl;

};