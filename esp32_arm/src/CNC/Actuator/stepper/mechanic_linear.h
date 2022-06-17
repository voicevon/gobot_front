#pragma once
#include "mechanic.h"

class ActuatorMechanic_StepperLinear:public ActuatorMechanicStepper{
    public:
        float GetStepsPerCncUnit() override;

    protected:
        float _motor_gear_pitch_in_mm;

    private:


};