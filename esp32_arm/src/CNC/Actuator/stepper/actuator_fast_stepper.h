#pragma once
#include "../cnc_actuator_base.h"
#include "FastAccelStepper.h"


class CncActuatorFastStepper: public CncActuatorBase{
    public:
        void SpinOnce() override;
        void ForceStop() override;
        void PrintOut(const char* title);
        void LinkStepper(FastAccelStepper* stepper);

        void UpdateMovement(MoveBlock_SingleActuator* line) override;
        void SetCurrentPositionAs(float new_position) override;

    protected:

    private:
        FastAccelStepper* _stepper;

};