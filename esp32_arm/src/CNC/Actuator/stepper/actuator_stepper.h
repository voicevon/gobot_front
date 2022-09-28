#pragma once
#include "../actuator_base.h"
#include "ESP32Step/src/Stepper.h"


class ActuatorStepper: public ActuatorBase{
    public:
        void PrintOut(const char* title);
        void LinkStepper(Stepper* stepper, float steps_per_cnc_unit);
        Stepper* GetLinkedStepper(){return this->_stepper;};

        float GetCurrentPosition() override;
        void SetCurrentPositionAs(float position_in_cnc_unit) override;
        // void SetTargetPositionTo(bool is_absolute_position, float position_in_cnc_unit) override;
        void UpdateMovement(MoveBlock_SingleActuator* line) override;

        float GetAbsDistanceToTarget_InCncUnit() override;

        //speed and acceleration
        // void Stop() override{};   // Implatcate by StepControl.  //TODO:  remove this from base class, and all actuator will not know: start(), stop()
        void UpdateTargetPositionFromCurrent() override{};
        float GetSpeed() override {return this->__speed;};
        // void SetSpeed(float speed_in_cnc_unit) override;
        // void SetAccelleration(float accelleration_in_cnc_unit) override;
        

    protected:
        Stepper* _stepper;

    private:
        float __steps_per_cnc_unit;
        float __current_cnc_position_in_rad;
        float __distance_to_target;
        float __speed;

};