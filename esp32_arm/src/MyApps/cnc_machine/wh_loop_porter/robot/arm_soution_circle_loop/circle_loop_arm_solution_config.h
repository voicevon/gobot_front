#pragma once


class Twh_Circleloop_Armsolution_Config{
    public:

        // One column equal to one teeth box.
        float Slope_Steps_per_box(){
            float steps_per_motor_round  = 360.0f / __motor_step_angle_in_degree * __stepper_driver_micro_steps;
            float steps_per_chain_gear_round = steps_per_motor_round * __motor_gear_ratio;
            float steps_per_chain_gear_teeth = steps_per_chain_gear_round / __chain_gear_teeth_count;

            float slope = steps_per_chain_gear_teeth / __teeth_count_per_box;   
            Logger::Info("Twh_Circleloop_Armsolution_Config");
            Logger::Print("slope",slope);
            return slope;
        };

    protected:
        float __motor_step_angle_in_degree = 1.8;  // confirmed
        int __stepper_driver_micro_steps = 16;    
        float __motor_gear_ratio = 27.0f;
        
        int __chain_gear_teeth_count = 20;
        int __teeth_count_per_box = 2;
};



