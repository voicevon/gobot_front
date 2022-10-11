#pragma once

#include "CNC/arm_solution/kinematic_config.h"

class CncSolution_CoreYZConfigBase: public KinematicConfig{
    public:
        float max_speed_alpha_beta;
        float max_acceleration_alpha_beta;
     
        float Homed_position_z = 0;
        float Homed_position_y = 0;
        int Homing_speed_alpha_beta; // MAX_SPEED_HOMING_ALPHA 200 * MICRO_STEPS   
        int Homing_acceleration_alpha_beta ;
        // void PrintOut(const char* title) override;

    protected:
};



