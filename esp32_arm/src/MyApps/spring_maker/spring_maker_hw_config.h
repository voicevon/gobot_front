#pragma once

class SpringMakerHardwareConfig{
    public:
        float steps_per_rad_for_a;
        float max_speed_alpha;
        float max_acceleration_alpha;
     
        float Homed_position_fk_A = 0;
        int Homing_speed_alpha;        // MAX_SPEED_HOMING_ALPHA 200 * MICRO_STEPS   
        int Homing_acceleration_alpha ;
        void Init();
        void PrintOut();

    private:
        int STEPPER_DRIVER_MICRO_STEPS;
        float MOTOR_DEGREE_PER_STEP;
        float MOTOR_GEARBOX_RATIO_ALPHA;
        float motor_steps_per_round;

};



