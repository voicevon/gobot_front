#pragma once
#include "CNC/cnc_machine_base.h"

class CncFiveBarMachine: public CncMachineBase{
    public:
        // link length in mm
        CncFiveBarMachine(){};
        void PrintOut() override;
        virtual float GetHomingVelocity(char axis_name);

        const float& Homed_position_alpha_in_rad = _HOMED_POSITION[AXIS_ALPHA];
        const float& Homed_position_beta_in_rad = _HOMED_POSITION[AXIS_BETA];

        // 191.0 is the Length between the centers of two motors 
        const float& LINK_0 = _LINK_0_LENGTH;  //  = 191.0 / 2 ;
        // Length from motor to passive joints   
        const float& LINK_A = _LINK_A_LENGTH; // = 285.18;  
        // Length from passive joints to end effector 
        const float& LINK_B = _LINK_B_LENGTH; // 384.51;  

    protected:
        float _LINK_0_LENGTH;
        float _LINK_A_LENGTH;
        float _LINK_B_LENGTH;
};