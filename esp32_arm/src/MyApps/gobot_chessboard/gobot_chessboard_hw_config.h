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
.         ----------------------o----------------------o-----------------> X
.                               |                      |
.                               |<------- LINK0 ------>|



*/  



#define PIN_LED_POWER_2112 12
#define PIN_LED_B_2112 25
#define PIN_LED_C_2112 26
#define PIN_LED_F_2112 27
#define PIN_LED_H_2112 14



#define PIN_HOME_ALHPA  15     //??
#define PIN_ALPHA_DIR 19
#define PIN_ALPHA_STEP 5

#define PIN_HOME_BETA  2      //??
#define PIN_BETA_DIR 17
#define PIN_BETA_STEP 4


#define PIN_ENDER_COIL_2112 32
#define PIN_ENDER_COIL_EXT_2112 33

#define HOME_POSITION_ALPHA 3.3
#define HOME_POSITION_BETA 3.3

#define PIN_EEF_SERVO 21
#define PIN_EEF_A  18
#define PIN_EEF_B  19
#define PIN_ALPHA_ENABLE 18
#define PIN_BETA_ENABLE 16

#define MAX_STEPS_PER_SECOND_BETA 500 * 16 // MICRO_STEPS
#define MAX_ACCELERATION_BETA 200 * 16 //MICRO_STEPS
#define MAX_STEPS_PER_SECOND_ALPHA 1200 * 16 //MICRO_STEPS
#define MAX_ACCELERATION_ALPHPA 600 * 16 //MICRO_STEPS

// #define STEPS_PER_RAD 3056     // 200 * 16 * 6 / (3.1415927 * 2)
// #define  1000    // unit?

class GobotChessboardHardwareConfig{
    public:
        // link length in mm
        //  unit is mm
        // #define LINK_0 191.0  // Length between origin and the two motors
        // #define LINK_A 285.18 // Length from motor to passive joints
        // #define LINK_B 384.51 // Length from passive joints to end effector
        int LINK_0 = 191.0;   // Length between origin and the two motors
        int LINK_A = 285.18;   // Length from motor to passive joints
        int LINK_B = 384.51;  // Length from passive joints to end effector
        float steps_per_rad = 3056;
        int MOTOR_MAX_SPEED = 1000;


        float Homed_position_alpha_in_degree = 6;
        float Homed_position_beta_in_degree = 138;
        float Homed_position_x = 22;
        float Homed_position_y = 56;
        int Homing_acceleration_alpha; // ACCELERATION_HOMIMG_ALPHA 200 * MICRO_STEPS
        int Homing_speed_alpha; // MAX_SPEED_HOMING_ALPHA 200 * MICRO_STEPS   
        int Homing_acceleration_beta ;
        int Homing_speed_beta;
        
        void Init();
        void PrintOut();
};