#pragma once
#include "../cnc_actuator_base.h"
#include "FastAccelStepper.h"


class CncActuatorFastStepper: public CncActuatorBase{
    public:
        void SpinOnce() override;
        void ForceStop_G28_Only(bool update_target_position = true) override;
        void PrintOut(const char* title, uint32_t count_up_max);
        void LinkStepper(FastAccelStepper* stepper);

        void UpdateMovement(MoveBlock_SingleActuator* line) override;
        void SetCurrentPositionAs(float new_position) override;
        float GetCurrentPosition() override;

    protected:

    private:
        FastAccelStepper* _stepper;

};