#pragma once


#define MICRO_STEPS 1
#define STEPS_PER_ROUND_MOTOR 514 * MICRO_STEPS  //Can be calibrated at step=1
#define GEARBOX_RATIAO_ALPHA 9.0   // big_gear_teeth ==90 / small_gear_teeth ==10
#define GEARBOX_RATIAO_BETA 2.6     // big_gear_teeth ==26/ small_gear_teeth == 10

#define STEPS_PER_RAD_ALPHA 736.25 // STEPS_PER_ROUND_MOTOR*GEARBOX_RATIAO_ALPHA/2/PI   //730 
#define MAX_STEPS_PER_SECOND_ALPHA 1200 * MICRO_STEPS
#define MAX_ACCELERATION_ALPHPA 600 * MICRO_STEPS


#define STEPS_PER_RAD_BETA 212.7 // STEPS_PER_ROUND_MOTOR * GEARBOX_RATIAO_BETA/2/PI   // 230 
#define MAX_STEPS_PER_SECOND_BETA 500 * MICRO_STEPS
#define MAX_ACCELERATION_BETA 200 * MICRO_STEPS



class GobotHouseHardwareConfig{
    public:
        int driver_microsteps = 1;
        float motor_steps_per_round = 514;
        // float GEARBOX_RATIAO_ALPHA 9.0   // big_gear_teeth ==90 / small_gear_teeth ==10
        // float GEARBOX_RATIAO_BETA 2.6     // big_gear_teeth ==26/ small_gear_teeth == 10

        //The length of Master linkage, unit is mm
        float LINK_A = 75.0;
        //The length of slave linkage, unit is mm
        float LINK_B = 75.0;
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
    private:
        float MOTOR_MICRO_STEPS = 16;
        float MOTOR_STEP_ANGLE = 1.8;
        float GEAR_BOX_RATIO = 6.5;
};



