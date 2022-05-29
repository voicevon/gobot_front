#pragma once





// #include "board_ver1.2.h"
// #include "MyBoards/cnc_dual_stepper_2201/board_ver1.2.h"
#include "MyBoards/gobot_main/board_ver1.2.h"
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