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


class Twh_ArmSolution_Config{
    public:

        float arm_length = 190;                 //unit is mm, confirmed

        float max_speed_alpha_beta;
        float max_acceleration_alpha_beta;

        float middle_kinematic_minimuim_X = 123;
        float arm_gear_circle_length;
        float master_slope_steps_per_mm = 1; 

        Twh_ArmSolution_Config(){
            arm_gear_circle_length = __arm_gear_pitch_in_mm * __arm_gear_teeth_count;
        }

    private:
        float __arm_gear_pitch_in_mm = 3.0f;    // confirmed
        int __arm_gear_teeth_count = 24;        //confiemd
};



