#pragma once

#include "CNC/cnc_machine_base.h"

class CncCoreYZMachine: public CncMachineBase{
    public:
        float steps_per_mm_for_y;
        float steps_per_mm_for_z;

        float max_speed_alpha_beta;
        float max_acceleration_alpha_beta;
     
        float Homed_position_z = 0;
        float Homed_position_y = 0;
        int Homing_speed_alpha_beta; // MAX_SPEED_HOMING_ALPHA 200 * MICRO_STEPS   
        int Homing_acceleration_alpha_beta ;
        void PrintOut() override;

    protected:
        int STEPPER_DRIVER_MICRO_STEPS;
        float MOTOR_DEGREE_PER_STEP;
        float MOTOR_GEARBOX_RATIO_ALPHA_BETA;
        int MOTOR_PULLEY_TEETH_COUNT;
        int  MOTOR_PULLEY_PITCH;
        float steps_per_motor_round;
};



