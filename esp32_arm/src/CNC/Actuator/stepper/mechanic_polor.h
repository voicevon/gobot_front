#pragma once


#include "mechanic.h"

class ActuatorStepper_PolorConfig: public ActuatorStepperConfig{
    public:
        float GetStepsPerCncUnit() override;


    protected:


    private:
};
