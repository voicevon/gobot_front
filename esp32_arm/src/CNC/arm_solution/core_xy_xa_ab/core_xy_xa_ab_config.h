#pragma once


class Core_XY_XA_ab_config{
    public:
    
        float max_speed_alpha_beta;
        float max_acceleration_alpha_beta;
     
        float master_slope_steps_per_mm = 1; 
        float slave_gear_pitch_in_mm = 3;
        float slave_gear_teeth_count = 20;
        float slave_gear_circle_length = 123;
        float arm_length = 11;
        float middle_kinematic_minimuim_X = 123;


        void PrintOut(){};


    protected:

};



