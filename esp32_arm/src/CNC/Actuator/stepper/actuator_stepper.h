#pragma once
#include "../actuator_base.h"
#include "ESP32Step/src/Stepper.h"


class ActuatorStepper: public ActuatorBase{
    public:
    // float actuator_position = 1.0f * this->_stepper->getPosition() / this->__steps_per_cnc_unit;
        void SpinOnce() override{this->_current_position=this->_stepper->getPosition();};
        void ForceStop() override{};
        void PrintOut(const char* title);
        void LinkStepper(Stepper* stepper, float steps_per_cnc_unit);
        Stepper* GetLinkedStepper(){return this->_stepper;};

        void UpdateMovement(MoveBlock_SingleActuator* line) override;


    protected:
        Stepper* _stepper;

    private:
        float __steps_per_cnc_unit;
        float __distance_to_target;
        float __speed;   // todo:  remove this

};