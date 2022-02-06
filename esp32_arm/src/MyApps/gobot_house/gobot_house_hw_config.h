#pragma once
#include <Arduino.h>

#define PIN_ALPHA_ENABLE 18
#define PIN_BETA_ENABLE 16
#define PIN_MICRIO_STEP_2 21
#define PIN_MICRIO_STEP_1 22
#define PIN_MICRIO_STEP_0 23


#define PIN_ENDER_COIL_2109 32
#define PIN_ENDER_COIL_EXT_2109 33



class GobotHouseHardwareConfig{
    public:
        // The value 514.0 is measured/Calibrated from running 10 rounds, Not calculated from anywhere.
        // This value should be calculated from motor's datasheet.  But can not get datasheet for 28-BGY motor.
        float motor_steps_per_round = 514.0;

        //The length of Master linkage, unit is mm
        float LINK_A = 75.0;
        //The length of slave linkage, unit is mm
        float LINK_B = 75.0;

        // Should be 736.25, validated.
        float STEPS_PER_RAD_ALPHA = this->motor_steps_per_round * this->GEAR_BOX_RATIO_ALPHA / TWO_PI;  

        float MAX_ACCELERATION_ALPHPA = 600 * this->MOTOR_MICRO_STEPS;
        float MAX_STEPS_PER_SECOND_ALPHA = 1200 * this->MOTOR_MICRO_STEPS;

        float MAX_STEPS_PER_SECOND_BETA =  500 * this->MOTOR_MICRO_STEPS;
        float MAX_ACCELERATION_BETA = 200 * this->MOTOR_MICRO_STEPS;
        // Should be 212.7, validated. 
        float STEPS_PER_RAD_BETA =  this->motor_steps_per_round * this->GEAR_BOX_RATIO_BETA / TWO_PI;    

        float Homed_position_alpha_in_degree = 6;
        float Homed_position_beta_in_degree = 138;
        int Homing_acceleration_alpha; // ACCELERATION_HOMIMG_ALPHA 200 * MICRO_STEPS
        int Homing_speed_alpha; // MAX_SPEED_HOMING_ALPHA 200 * MICRO_STEPS   
        int Homing_acceleration_beta ;
        int Homing_speed_beta;
        void Init();
        void PrintOut();
    private:
        float MOTOR_MICRO_STEPS = 1;
        // big_gear_teeth == 90 / small_gear_teeth == 10
        float GEAR_BOX_RATIO_ALPHA = 9.0;
        // big_gear_teeth == 26 / small_gear_teeth == 10
        float GEAR_BOX_RATIO_BETA = 2.6;
};



