#pragma once
#include "../actuator_base.h"
#include "ESP32Step/src/Stepper.h"

class ActuatorStepper: public ActuatorBase{
    public:
        void LinkStepper(Stepper* stepper){this->__stepper=stepper;};
        Stepper* GetLinkedStepper(){return this->__stepper;};
        
        float GetCurrentPosition_InCncUnit() override;
        void SetPosition(float position_in_cnc_unit) override;
        void MoveTo(bool is_absolute_position, float position_in_cnc_unit) override;
        float GetDistanceToTarget_InCncUnit() override;

    protected:

    private:
        Stepper* __stepper;
        float steps_per_cnc_unit = 1.234;
        bool inverse_direction = false;

};