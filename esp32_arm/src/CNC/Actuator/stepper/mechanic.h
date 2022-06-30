#pragma once

// #include "../actuator_mechanic_base.h"
#include "../actuator_config_base.h"
#include "MyBoards/const.h"

class ActuatorStepperConfig: public ActuatorConfigBase{
    public: 
        virtual float GetStepsPerCncUnit();
        
    protected:
        int _micro_steps_on_stepper_driver;
        float _motor_step_angle_in_degree;
        int _motor_gear_teeth_count;
        int _slave_pulley_teeth_count;

    private:
};