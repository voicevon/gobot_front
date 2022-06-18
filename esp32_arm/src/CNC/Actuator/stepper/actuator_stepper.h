#pragma once
#include "../actuator_base.h"
#include "ESP32Step/src/Stepper.h"
#include "mechanic.h"

class ActuatorStepper: public ActuatorBase{
    public:
        void LinkStepper(Stepper* stepper, ActuatorMechanicStepper* mechanic);
        Stepper* GetLinkedStepper(){return this->__stepper;};
        
        float GetCurrentPosition_InCncUnit() override;
        void SetPosition(float position_in_cnc_unit) override;
        void MoveTo(bool is_absolute_position, float position_in_cnc_unit) override;
        float GetDistanceToTarget_InCncUnit() override;
        void SetSpeed(float speed_per_second) override;
        // bool MotorIsMoving(char moto_name) override;

    protected:

    private:
        Stepper* __stepper;
        float __steps_per_cnc_unit;
        float __fit_into_joint_range(float cnc_position);

};