#pragma once

#include "../actuator_mechanic_base.h"
#include "MyBoards/const.h"

class ActuatorMechanicStepper: public ActuatorMechanicBase{
    public: 
        virtual float GetStepsPerCncUnit();

    protected:
        int _micro_steps_on_stepper_driver;
        int _motor_step_angle_in_degree = 1.8;
        int _motor_gear_teeth_count;
        int _slave_pulley_teeth_count;

    private:
};