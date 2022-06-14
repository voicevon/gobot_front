#pragma once


#include "CNC/cnc_machine_base.h"
// #include "CNC/cnc_config_base.h"

class CncScaraMachine: public CncMachineBase{
    public:
        // value is vary for each gobot-house robot.
        float Homed_position_alpha_in_degree;
        // value is vary for each gobot-house robot.
        float Homed_position_beta_in_degree;
        float motor_steps_per_round;

        //The length of Master linkage, unit is mm
        float LINK_A = 75.0;
        //The length of slave linkage, unit is mm
        float LINK_B = 75.0;

        // when microstep==1 Should be 736.25 , validated.
        float STEPS_PER_RAD_ALPHA;  
        float MAX_ACCELERATION_ALPHPA; // = 600 * this->MOTOR_MICRO_STEPS;
        float MAX_STEPS_PER_SECOND_ALPHA; // = 1200 * this->MOTOR_MICRO_STEPS;    // tested 1200

        // [when microstep==1] Should be 212.7, validated. 
        float STEPS_PER_RAD_BETA; // =  this->motor_steps_per_round * this->GEAR_BOX_RATIO_BETA / TWO_PI;    
        float MAX_ACCELERATION_BETA; // = 200 * this->MOTOR_MICRO_STEPS;
        float MAX_STEPS_PER_SECOND_BETA; // =  500 * this->MOTOR_MICRO_STEPS; 

        int Homing_acceleration_alpha; 
        int Homing_speed_alpha;    
        int Homing_acceleration_beta ;
        int Homing_speed_beta;
        uint16_t EEF_Suck_Angle;
        uint16_t EEF_Release_Angle;

        
    protected:
        // Fixed by electronic wiring.
        float MOTOR_MICRO_STEPS = 16;
        // big_gear_teeth == 90 / small_gear_teeth == 10
        float GEAR_BOX_RATIO_ALPHA = 9.0;
        // big_gear_teeth == 26 / small_gear_teeth == 10
        float GEAR_BOX_RATIO_BETA = 2.6;

};