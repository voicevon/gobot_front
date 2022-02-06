#pragma once

/*
.                                          Y
.                              /           ^            \
.                             /            |             \  LINK2
.                            /             |              \
.                           /              |               \
.                           \              |               /
.                            \             |              /  LINK1
.                             \            |             /
.                              \  alpha    |            /  beta
.         ----------------------o---------(0,0)--------o-----------------> X
.                               |          |
.                               |<-LINK0 ->|



*/  

#define PIN_HOME_ALHPA  15     //??
#define PIN_ALPHA_DIR 19
#define PIN_ALPHA_STEP 5

#define PIN_HOME_BETA  2      //??
#define PIN_BETA_DIR 17
#define PIN_BETA_STEP 4

#define PIN_ALPHA_ENABLE 18
#define PIN_BETA_ENABLE 16

#define PIN_EEF_SERVO 21
#define PIN_EEF_A  18
#define PIN_EEF_B  19


#define MAX_STEPS_PER_SECOND_ALPHA_BETA 1200 * 16 //MICRO_STEPS
#define MAX_ACCELERATION_ALPHA_BETA 600 * 16 //MICRO_STEPS

// #define STEPS_PER_ROUND_MOTOR 123
class GobotChessboardHardwareConfig{
    public:
        // link length in mm
        //  unit is mm

        float MOTOR_STEPS_PER_ROUND = 360 / this->MOTOR_STEP_ANGLE * this->MOTOR_MICRO_STEPS;
        float STEPS_PER_RAD = this->MOTOR_STEPS_PER_ROUND * this->GEAR_BOX_RATIO / TWO_PI;
        // 191.0 is the Length between the centers of two motors 
        int LINK_0 = 191.0 / 2 ;
        // Length from motor to passive joints   
        int LINK_A = 285.18;  
        // Length from passive joints to end effector 
        int LINK_B = 384.51;  

        int MOTOR_MAX_SPEED = 1000;
        float Homed_position_alpha_in_degree = 180+2;
        float Homed_position_beta_in_degree = 46;
        int Homing_acceleration_alpha_beta = 200 * this->MOTOR_MICRO_STEPS; 
        int Homing_speed_alpha_beta = 200 * this->MOTOR_MICRO_STEPS; 
        
        void Init(){};
        void PrintOut(){};

    private:
        float MOTOR_MICRO_STEPS = 16;
        float MOTOR_STEP_ANGLE = 1.8;
        float GEAR_BOX_RATIO = 6.5;
};