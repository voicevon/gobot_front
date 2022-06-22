#pragma once
#include "../actuator_base.h"
#include "ESP32Step/src/Stepper.h"
#include "mechanic.h"

class ActuatorStepper: public ActuatorBase{
    public:
        void LinkStepper(Stepper* stepper, float steps_per_cnc_unit);
        Stepper* GetLinkedStepper(){return this->__stepper;};
        
        float GetCurrentPosition_InCncUnit() override;
        void SetCurrentPositionAs(float position_in_cnc_unit) override;
        void SetTargetPositionTo(bool is_absolute_position, float position_in_cnc_unit) override;
        float GetDistanceToTarget_InCncUnit() override;
        void SetSpeed(float speed_per_second) override;
        // bool MotorIsMoving(char moto_name) override;
        void Stop() override {};   // Implatcate by StepControl.

    protected:
        float _ConvertTo_ActuatorRange(float cnc_position) override;
        float _ConvertTo_CncRange(float actuator_position) override;
    private:
        Stepper* __stepper;
        float __steps_per_cnc_unit;
        float __current_cnc_position_in_rad;
        float __distance_to_target;

};