#pragma once


#include "config.h"

class ActuatorStepper_PolorConfig: public ActuatorStepperConfig{
    public:
        float GetStepsPerCncUnit() override;


    protected:


    private:
};
