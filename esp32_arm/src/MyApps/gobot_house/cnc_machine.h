#pragma once

#include "CNC/solution/cnc_scara/cnc_machine.h"


class GobotHouseMachine: public CncScaraMachine{
    public:
        void Init(char solution_id) override;
        // void Init();
        void PrintOut();
        int32_t GetStepsPerUnit(char axis_name) override;
        float GetHomingVelocity(char axis_name) override;
        
        // [when microstep==1] Should be 212.7, validated. 
        float STEPS_PER_RAD_BETA; // =  this->motor_steps_per_round * this->GEAR_BOX_RATIO_BETA / TWO_PI;    
        float MAX_ACCELERATION_BETA; // = 200 * this->MOTOR_MICRO_STEPS;
        float MAX_STEPS_PER_SECOND_BETA; // =  500 * this->MOTOR_MICRO_STEPS; 

    private:
            // Fixed by electronic wiring.
        float MOTOR_MICRO_STEPS = 16;
        // big_gear_teeth == 90 / small_gear_teeth == 10
        float GEAR_BOX_RATIO_ALPHA = 9.0;
        // big_gear_teeth == 26 / small_gear_teeth == 10
        float GEAR_BOX_RATIO_BETA = 2.6;
        int8_t HOMING_DIR_ALPHA = 1;
        int8_t HOMING_DIR_BETA = -1;
        float Homing_speed_beta;
};


