#pragma once

#include "../actuator_to_cnc_foumular_calculator_base.h"
#include "MyLibs/utility/logger.h"

class ActuatorEncoder_Calculator{
    public: 

        //Slope unit is:  steps_per_mm, or  steps_per_rad.
        float Get_Formular_Slope_raw_per_mm() ; 
        float Get_Formular_Slope_raw_per_rad() ;

        // float Get_steps_per_mm();

        int motor_driver_micro_steps;
        float motor_step_angle_in_degree;
        int motor_gear_teeth_count;
        int slave_pulley_teeth_count;

        float motor_gear_pitch_in_mm;
        
    protected:

    private:
};