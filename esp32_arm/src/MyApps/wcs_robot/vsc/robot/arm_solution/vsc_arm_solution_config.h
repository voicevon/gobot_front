#pragma once


class Vsc_Armsolution_Config{
    public:
        float Slope_chain_rad_per_sensor_rad(){
            //For one circle of chains
            float motor_turns = 1.0f * __chain_pitch_count / __motor_wheel_gear_teeth  ;
            //For One circle of motor
            float encoder_turns = 1.0f * __motor_sensor_gear_teeth / __encoder_gear_teeth_;
            
            float slope = 1.0f / (motor_turns * encoder_turns); 
            return slope;
        };

    protected:
        // float __pitch_in_mm = 12.7f;   // useless, because we care chain_rad as cnc unit.
        // int __encoder_lines_per_circle = 200;  //useless, because encoder get angle in rad.

        int __chain_pitch_count = 368;
        int __encoder_gear_teeth_ = 50;
        int __motor_sensor_gear_teeth = 50;

        int __motor_wheel_gear_teeth = 10;
        int __big_wheel_gear_teeth = 56;
};



