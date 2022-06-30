#pragma once
#include "mechanic.h"

class ActuatorStepper_LinearConfig :public ActuatorStepperConfig{
    public:
        float GetStepsPerCncUnit() override;

    protected:
        float _motor_gear_pitch_in_mm;

    private:


};