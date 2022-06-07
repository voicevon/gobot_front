#pragma once
#include "CNC/cnc_core_yz/cnc_machine.h"

class BoxCarrierMachine: public CncCoreYZMachine{
    public:
        void Init(char solution_id) override;
        void PrintOut();
        int32_t GetStepsPerUnit(char axis_name) override;

    private:
        void __Init_Fast();

    //     float steps_per_mm_for_y;
    //     float steps_per_mm_for_z;

    //     float max_speed_alpha_beta;
    //     float max_acceleration_alpha_beta;
     
    //     float Homed_position_z = 0;
    //     float Homed_position_y = 0;
    //     int Homing_speed_alpha_beta; // MAX_SPEED_HOMING_ALPHA 200 * MICRO_STEPS   
    //     int Homing_acceleration_alpha_beta ;
    //     void Init();
    //     void PrintOut();

    // private:
    //     int STEPPER_DRIVER_MICRO_STEPS;
    //     float MOTOR_DEGREE_PER_STEP;
    //     float MOTOR_GEARBOX_RATIO_ALPHA_BETA;
    //     float steps_per_motor_round;

};


