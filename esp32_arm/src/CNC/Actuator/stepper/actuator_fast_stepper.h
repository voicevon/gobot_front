#pragma once
#include "../cnc_actuator_base.h"
#include "FastAccelStepper.h"
#include "actuator_stepper_calculator.h"


class CncActuatorFastStepper: public CncActuatorBase{
    public:
    // float actuator_position = 1.0f * this->_stepper->getPosition() / this->__steps_per_cnc_unit;
        // void SpinOnce() override{this->_current_position=this->_stepper->getPosition();};
        void SpinOnce() override{this->_current_position=this->_stepper->getCurrentPosition();};
        void ForceStop() override;
        void PrintOut(const char* title);
        void LinkStepper(FastAccelStepper* stepper, float steps_per_cnc_unit);
        FastAccelStepper* GetLinkedStepper(){return this->_stepper;};

        void UpdateMovement(MoveBlock_SingleActuator* line) override;


    protected:
        FastAccelStepper* _stepper;

    private:
        float __steps_per_cnc_unit;
        float __distance_to_target;
        float __speed;   // todo:  remove this

};