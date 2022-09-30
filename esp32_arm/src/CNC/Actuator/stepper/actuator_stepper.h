#pragma once
#include "../actuator_base.h"
#include "ESP32Step/src/Stepper.h"


class ActuatorStepper: public ActuatorBase{
    public:
        void SpinOnce() override{};
        void ForceStop() override{};
        void PrintOut(const char* title);
        void LinkStepper(Stepper* stepper, float steps_per_cnc_unit);
        Stepper* GetLinkedStepper(){return this->_stepper;};

        float GetCurrentPosition() override;
        void SetCurrentPositionAs(float position_in_cnc_unit) override;
        void UpdateMovement(MoveBlock_SingleActuator* line) override;

        float GetAbsDistanceToTarget_InCncUnit() override;

        void UpdateTargetPositionFromCurrent() override{};
        float GetSpeed() override {return this->__speed;};
        

    protected:
        Stepper* _stepper;

    private:
        float __steps_per_cnc_unit;
        float __current_cnc_position_in_rad;
        float __distance_to_target;
        float __speed;

};