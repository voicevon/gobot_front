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

        float middle_kinematic_minimuim_X = 123;

        float arm_slope_steps_per_rad; 
        float linear_slope_steps_per_mm;

        Twh_ArmSolution_Config(){
            float steps_per_motor_round  = 360.0f / __motor_gear_teeth_count * __stepper_driver_micro_steps;
            float motor_gear_circle_length = __gear_pitch_in_mm * __motor_gear_teeth_count;
            linear_slope_steps_per_mm = steps_per_motor_round / motor_gear_circle_length /2;
    
            float arm_gear_circle_length = __gear_pitch_in_mm * __arm_gear_teeth_count;
            arm_slope_steps_per_rad = linear_slope_steps_per_mm * arm_gear_circle_length / TWO_PI ;

        }

    private:
        int __motor_step_angle_in_degree = 1.8;   // confirmed
        int __motor_gear_teeth_count = 15;        // confirmed
        int __stepper_driver_micro_steps = 32;    //  16 or 32  ?
        float __gear_pitch_in_mm = 3.0f;          // confirmed
        int __arm_gear_teeth_count = 24;          // confirmed
         
};



