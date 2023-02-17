#pragma once


class Sillicon_Pump_Armsolution_Config{
    public:

        // One column equal to one teeth box.
        float Slope_Steps_per_box(){
            float steps_per_motor_round  = 360.0f / __motor_step_angle_in_degree * __stepper_driver_micro_steps;
            float steps_per_chain_gear_round = steps_per_motor_round * __slave_pulley_teeth_count / __motor_pulley_teeth_count;
            float steps_per_chain_gear_teeth = steps_per_chain_gear_round / __chain_gear_teeth_count;
         
            float slope = steps_per_chain_gear_teeth / 2;   // one box occupy two gear teeth.
            Logger::Info("Twh_Circleloop_Armsolution_Config");
            Logger::Print("slope",slope);
            return slope;
        };

    protected:
        // float __pitch_in_mm = 12.7f;   // useless, because we care chain_rad as cnc unit.
        // int __chain_pitch_count = 60;   // 59??  useless.
 
        float __motor_step_angle_in_degree = 1.8;  // confirmed
        int __stepper_driver_micro_steps = 16;    

        int __motor_pulley_teeth_count = 20;        
        int __slave_pulley_teeth_count = 52;   // to be confirmed

        int __chain_gear_teeth_count = 10;
};



