#pragma once
#include "../cnc_actuator_base.h"
#include "ESP32Step/src/Stepper.h"
#include "actuator_stepper_calculator.h"

class CncActuatorStepper: public CncActuatorBase{
    public:
    // float actuator_position = 1.0f * this->_stepper->getPosition() / this->__steps_per_cnc_unit;
        void SpinOnce() override{};
        void ForceStop() override{};
        void PrintOut(const char* title);
        void LinkStepper(Stepper* stepper, float steps_per_cnc_unit);
        // Stepper* GetLinkedStepper(){return this->_stepper;};

        void UpdateMovement(MoveBlock_SingleActuator* line) override;


    protected:
        Stepper* _stepper;

    private:
        float __steps_per_cnc_unit;
        float __distance_to_target;
        float __speed;   // todo:  remove this

};