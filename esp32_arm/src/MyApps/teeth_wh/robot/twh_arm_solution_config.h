#pragma once

/*
.                           Y (0 degree)
.                           ^
.                           |
.        (-45 degree)  \    |    / (45 degree)
.                       \   |   /
.                        \  |  /
.                         \ | / 
.              -----------(0,0)----------->X
*/


class Core_XY_XA_ab_config{
    public:
    
        float max_speed_alpha_beta;
        float max_acceleration_alpha_beta;
     
        float master_slope_steps_per_mm = 1; 
        float slave_gear_pitch_in_mm = 3;    // confirmed
        float slave_gear_teeth_count = 24;  //confiemd
        float slave_gear_circle_length = 123;
        float arm_length = 190;   //unit is mm, confirmed
        float middle_kinematic_minimuim_X = 123;


        void PrintOut(){};


    protected:

};



