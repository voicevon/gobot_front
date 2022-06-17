#pragma once


#include "mechanic.h"

class ActuatorMechanic_StepperPolor: public ActuatorMechanicStepper{
    public:
        float GetStepsPerCncUnit() override;


    protected:


    private:
};
