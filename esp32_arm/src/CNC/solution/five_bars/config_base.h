#pragma once
#include "../solution_config_base.h"

class CncFiveBarConfig: public CncSolutionConfigBase{
    public:
        const float& Homed_position_alpha_in_rad = _HOMED_POSITION[AXIS_ALPHA];
        const float& Homed_position_beta_in_rad = _HOMED_POSITION[AXIS_BETA];

        const float& LINK_0 = _LINK_0_LENGTH;  //  = 191.0 / 2 ; 
        const float& LINK_A = _LINK_A_LENGTH; // = 285.18;  
        const float& LINK_B = _LINK_B_LENGTH; // 384.51;  

    protected:
        float _LINK_0_LENGTH;
        float _LINK_A_LENGTH;
        float _LINK_B_LENGTH;
};