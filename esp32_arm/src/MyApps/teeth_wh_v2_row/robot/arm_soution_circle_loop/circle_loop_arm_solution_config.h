#pragma once


class Twh2_Circleloop_Armsolution_Config{
    public:

        // One column equal to one teeth box.
        float Slope_Steps_per_column(){
            float steps_per_motor_round  = 360.0f / __motor_step_angle_in_degree * __stepper_driver_micro_steps;
            // float motor_gear_circle_length = __gear_pitch_in_mm * __motor_gear_teeth_count;
            // linear_slope_steps_per_mm = steps_per_motor_round / motor_gear_circle_length ;   // = 71.111 steps_per_mm 
    
            // float arm_gear_circle_length_in_mm = __gear_pitch_in_mm * __arm_gear_teeth_count;
            // arm_slope_steps_per_rad = - linear_slope_steps_per_mm * arm_gear_circle_length_in_mm / TWO_PI ;
 
            // //For one circle of chains
            // float motor_turns = 1.0f * __chain_pitch_count / __motor_wheel_gear_teeth  ;
            // //For One circle of motor
            // float encoder_turns = 1.0f * __motor_sensor_gear_teeth / __encoder_gear_teeth_;
            
            float slope = 1.0f ; 
            return slope;
        };

    protected:
        // float __pitch_in_mm = 12.7f;   // useless, because we care chain_rad as cnc unit.
        // int __encoder_lines_per_circle = 200;  //useless, because encoder get angle in rad.

        float __motor_step_angle_in_degree = 1.8;  // confirmed
        int __stepper_driver_micro_steps = 16;    
        int __motor_gear_teeth_count = 15;        

        int __slave_pulley_teeth_count = 11;
        int __slave_chain_gear_teeth_count = 10;
        int __chain_pitch_count = 368;
        // int __encoder_gear_teeth_ = 50;
        // int __motor_sensor_gear_teeth = 50;

        // int __motor_wheel_gear_teeth = 10;
        // int __big_wheel_gear_teeth = 56;
};



