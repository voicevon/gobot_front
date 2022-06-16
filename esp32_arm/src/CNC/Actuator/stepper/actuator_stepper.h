#pragma once
#include "../actuator_base.h"
#include "ESP32Step/src/Stepper.h"

class ActuatorStepper: public ActuatorBase{
    public:
        void LinkStepper(Stepper* stepper){this->__stepper=stepper;};
        float GetCurrentPosition() override;
        void SetPosition(float position) override;
        void MoveTo(bool is_absolute_position, float position) override;
        Stepper* GetLinkedStepper(){return this->__stepper;};
    protected:

    private:
        Stepper* __stepper;

};