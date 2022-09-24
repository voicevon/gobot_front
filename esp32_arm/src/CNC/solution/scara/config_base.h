#pragma once
#include "../kinematic_config.h"

class CncSolution_ScaraConfigBase: public KinematicConfig{
    public:
        // CncScaraMachine(){};   // This is a abstract class, no constructor.
        // const float& Homed_position_alpha_in_rad = _HOMED_POSITION[AXIS_ALPHA];
        // const float& Homed_position_beta_in_rad = _HOMED_POSITION[AXIS_BETA];


        const float& LINK_A = _LINK_A_LENGTH;
        const float& LINK_B = _LINK_B_LENGTH;

    protected:
        float _LINK_A_LENGTH;
        float _LINK_B_LENGTH;

};