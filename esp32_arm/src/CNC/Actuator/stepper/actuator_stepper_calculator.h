#pragma once

#include "../actuator_to_cnc_foumular_calculator_base.h"
#include "MyBoards/const.h"
// #include "MyLibs/basic/logger.h"

class ActuatorStepper_Calculator: public ActuatorToCncFormular_CalculatorBase{
    public: 

        //Slope unit is:  steps_per_mm, or  steps_per_rad.
        float GetActuatorToCncFormular_Slope_raw_per_mm() override; 
        float GetActuatorToCncFormular_Slope_raw_per_rad() override;

        float Get_steps_per_mm();

        int _micro_steps_on_stepper_driver;
        float _motor_step_angle_in_degree;
        int _motor_gear_teeth_count;
        int _slave_pulley_teeth_count;

        float _motor_gear_pitch_in_mm;
        
    protected:

    private:
};