#pragma once


#include "mechanic.h"

class ActuatorMechanic_StepperPolor: public ActuatorConfigStepper{
    public:
        float GetStepsPerCncUnit() override;


    protected:


    private:
};
