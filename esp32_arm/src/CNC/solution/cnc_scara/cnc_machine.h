#pragma once
#include "CNC/cnc_machine_base.h"

class CncScaraMachine: public CncMachineBase{
    public:
        CncScaraMachine(){};
        virtual float GetHomingVelocity(int8_t axis_name);
        // const float& Homed_position_alpha_in_degree = _homed_position_alpha_in_degree;
        const float& Homed_position_alpha_in_rad = _HOMED_POSITION[AXIS_ALPHA];
        // const float& Homed_position_beta_in_degree = _homed_position_beta_in_degree;
        const float& Homed_position_beta_in_rad = _HOMED_POSITION[AXIS_BETA];

        const float& LINK_A = _LINK_A_LENGTH;
        const float& LINK_B = _LINK_B_LENGTH;

        // float MAX_ACCELERATION_ALPHPA; 
        // float MAX_STEPS_PER_SECOND_ALPHA; 

        // int Homing_acceleration_alpha; 

        
    protected:
        float _LINK_A_LENGTH;
        float _LINK_B_LENGTH;
        // float _homed_position_alpha_in_degree;
        // float _homed_position_beta_in_degree;

        // int _homing_speed_alpha_degree_per_second;    
        // float _homing_speed_beta_degree_per_second;
        // int Homing_acceleration_beta;
        // int8_t HOMING_DIR_ALPHA_IS_TO_MAX ;
        // int8_t HOMING_DIR_BETA_IS_TO_MAX ;


};