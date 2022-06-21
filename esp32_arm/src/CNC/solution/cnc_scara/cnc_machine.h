#pragma once
#include "CNC/cnc_machine_base.h"

class CncScaraMachine: public CncMachineBase{
    public:
        CncScaraMachine(){};
        virtual float GetHomingVelocity(char axis_name);
        float Homed_position_alpha_in_degree;
        float Homed_position_beta_in_degree;

        float LINK_A;
        float LINK_B;

        float MAX_ACCELERATION_ALPHPA; 
        float MAX_STEPS_PER_SECOND_ALPHA; 

        int Homing_acceleration_alpha; 

        
    protected:
        int _homing_speed_alpha_degree_per_second;    
        float _homing_speed_beta_degree_per_second;
        int Homing_acceleration_beta;
        int8_t HOMING_DIR_ALPHA_IS_TO_MAX ;
        int8_t HOMING_DIR_BETA_IS_TO_MAX ;

};