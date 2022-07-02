#pragma once
#include "cnc_solution_config.h"


BoxMover_CncSolutionConfig::BoxMover_CncSolutionConfig(){
        this->_IS_COMBINED_FK = true;

        this->max_acceleration_alpha_beta = 6000;
        this->max_speed_alpha_beta = 20000;

        this->Homing_speed_alpha_beta = 1300;
        this->Homing_acceleration_alpha_beta = 500;

        // this->steps_per_mm_for_z = 200;
        // this->steps_per_rad_for_w = 33;
}

void BoxMover_CncSolutionConfig::PrintOut(){
    Serial.print("\n\n===============================================");
    Serial.print("\nBoxMover_CncSolutionConfig::PrintOut()");
    // Serial.print("\n   STEPPER_DRIVER_MICRO_STEPS = ");
    // Serial.print(this->STEPPER_DRIVER_MICRO_STEPS);
    // Serial.print("\n   MOTOR_GEARBOX_RATIO_ALPHA_BETA = ");
    // Serial.print(this->MOTOR_GEARBOX_RATIO_ALPHA_BETA);

    // Serial.print("\n    steps_per_mm_for_z = ");
    // Serial.print(this->steps_per_mm_for_z);
    // Serial.print("\n    steps_per_rad_for_w = ");
    // Serial.print(this->steps_per_rad_for_w);
    // Serial.print("\n     Home_position (z,w) = (");
    Serial.print(this->Homed_position_z);
    Serial.print(" , ");
    Serial.print(this->Homed_position_w);
    Serial.print(" )");

    Serial.print("\n     Homing_speed_alpha_beta = ");
    Serial.print(this->Homing_speed_alpha_beta);
    Serial.print("\n     Homing_acceleration_alpha_beta = ");
    Serial.print(this->Homing_acceleration_alpha_beta);

    Serial.print("\n\n        max_speed_alpha_beta = ");
    Serial.print(max_acceleration_alpha_beta);
    Serial.print("\n        max_acceleration_alpha_beta = ");
    Serial.print(max_acceleration_alpha_beta);


    Serial.print("\n----------------------------------------------\n\n");
}
