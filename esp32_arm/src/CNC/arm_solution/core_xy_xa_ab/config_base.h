#pragma once

#include "../kinematic_config.h" 

class CncSolution_CoreAZConfigBase: public KinematicConfig{
    public:
    
        float max_speed_alpha_beta;
        float max_acceleration_alpha_beta;
     
        void PrintOut(){};


    protected:

};



