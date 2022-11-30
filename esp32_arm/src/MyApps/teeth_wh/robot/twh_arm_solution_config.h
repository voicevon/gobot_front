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

        // float max_speed_alpha_beta = 6000;
        // float max_acceleration_alpha_beta = 1000;

        float middle_kinematic_minimuim_X = 123;
        float master_slope_steps_per_mm = 1;
        float arm_gear_circle_length  =33;

        float arm_slope_steps_per_rad; 
        float linear_slope_steps_per_mm;

        Twh_ArmSolution_Config(){
            // float arm_gear_circle_length;
            float steps_per_motor_round  = 360.0f / __motor_gear_teeth_count * __stepper_driver_micro_steps;
            float motor_gear_circle_length = __gear_pitch_in_mm * __motor_gear_teeth_count;
            linear_slope_steps_per_mm = steps_per_motor_round / motor_gear_circle_length;

            float steps_per_arm_round = steps_per_motor_round / __motor_gear_teeth_count * __arm_gear_teeth_count;
            arm_slope_steps_per_rad = steps_per_arm_round / TWO_PI;

        }

    private:
        int __motor_step_angle_in_degree = 1.8;
        int __motor_gear_teeth_count = 15;
        int __stepper_driver_micro_steps = 32;
        float __gear_pitch_in_mm = 3.0f;    // confirmed
        int __arm_gear_teeth_count = 24;        //confiemd
         
};



