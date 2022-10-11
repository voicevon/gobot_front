#pragma once

#include "../kinematic_config.h" 

class Core_XA_ab_config: public KinematicConfig{
    public:
    
        float max_speed_alpha_beta;
        float max_acceleration_alpha_beta;
     
        float Homed_position_z = 12;
        float Homed_position_w = 34;
        int Homing_speed_alpha_beta; // MAX_SPEED_HOMING_ALPHA 200 * MICRO_STEPS   
        int Homing_acceleration_alpha_beta ;
        void PrintOut(){};


    protected:

};



