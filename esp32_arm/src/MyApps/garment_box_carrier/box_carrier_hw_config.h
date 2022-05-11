#pragma once

class BoxCarrierHardwareConfig{
    public:
        float steps_per_rad_for_w;
        float steps_per_mm_for_z;

        float max_speed_alpha_beta;
        float max_acceleration_alpha_beta;
     
        float Homed_position_z = 12;
        float Homed_position_w = 34;
        int Homing_speed_alpha_beta; // MAX_SPEED_HOMING_ALPHA 200 * MICRO_STEPS   
        int Homing_acceleration_alpha_beta ;
        void Init();
        void PrintOut();

    private:
        int STEPPER_DRIVER_MICRO_STEPS;
        float MOTOR_DEGREE_PER_STEP;
        float MOTOR_GEARBOX_RATIO_ALPHA_BETA;
        float motor_steps_per_round;

};



