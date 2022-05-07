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

#define PIN_ALPHA_DIR 4     // Ver0.x   19
#define PIN_ALPHA_STEP 16    //Ver0.x    5
#define PIN_ALPHA_ENABLE 17    // Ver0.x  18

#define PIN_BETA_DIR 5      //Ver 0.x   17
#define PIN_BETA_STEP 18    //Ver 0.x  4
#define PIN_BETA_ENABLE 19    // Ver0.1  16

#define PIN_HOME_ALPHA  15     //??
#define PIN_HOME_BETA  2      //??


#define PIN_EEF_SERVO 21
#define PIN_EEF_A  20
#define PIN_EEF_B  22


// #define  1200 * 16 //MICRO_STEPS
// #define MAX_ACCELERATION_ALPHA_BETA 600 * 16 //MICRO_STEPS

class GobotChessboardHardwareConfig{
    public:
        // link length in mm
        //  unit is mm
        GobotChessboardHardwareConfig();
        float MOTOR_STEPS_PER_SHAFT_ROUND;
        float STEPS_PER_RAD; 
        // 191.0 is the Length between the centers of two motors 
        float LINK_0;  //  = 191.0 / 2 ;
        // Length from motor to passive joints   
        float LINK_A; // = 285.18;  
        // Length from passive joints to end effector 
        float LINK_B; // 384.51;  

        // int MOTOR_MAX_SPEED;
        int MAX_STEPS_PER_SECOND_ALPHA_BETA;
        float MAX_ACCELERATION_ALPHA_BETA;
        float Homed_position_alpha_in_degree;
        float Homed_position_beta_in_degree;
        int Homing_acceleration_alpha_beta;
        int Homing_speed_alpha_beta;
        
        void Init();
        void PrintOut(const char* title);

    private:
        int STEPPER_DRIVER_MICRO_STEPS;
        float MOTOR_STEP_ANGLE;
        float GEAR_BOX_RATIO;
};