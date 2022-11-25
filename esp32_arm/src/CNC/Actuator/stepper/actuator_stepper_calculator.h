#pragma once

#include "../actuator_to_cnc_foumular_calculator_base.h"
#include "MyBoards/const.h"
// #include "MyLibs/basic/logger.h"

class ActuatorStepper_Calculator: public ActuatorToCncFormular_CalculatorBase{
    public: 

        //Slope unit is:  steps_per_mm, or  steps_per_rad.
        float Get_Formular_Slope_raw_per_mm() override; 
        float Get_Formular_Slope_raw_per_rad() override;

        // float Get_steps_per_mm();

        float motor_step_angle_in_degree;
        int motor_driver_micro_steps;
        float motor_gear_pitch_in_mm;
        int motor_gear_teeth_count;
        int slave_pulley_teeth_count;

        
    protected:

    private:
};