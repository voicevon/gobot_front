#pragma once


#include "mechanic.h"

class ActuatorMechanic_StepperPolor: public ActuatorMechanicStepper{
    public:
        uint32_t GetStepsPerCncUnit() override;


    protected:


    private:
};
