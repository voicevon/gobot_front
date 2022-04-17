#include "spring_maker_hw_config.h"
#include "HardwareSerial.h"


void SpringMakerHardwareConfig::Init(){
        this->STEPPER_DRIVER_MICRO_STEPS = 16;
        this->MOTOR_GEARBOX_RATIO_ALPHA = 21;
        this->MOTOR_DEGREE_PER_STEP = 1.8;
        this->max_acceleration_alpha = 6000;
        this->max_speed_alpha = 20000;

        this->motor_steps_per_round = 514.0 * this->STEPPER_DRIVER_MICRO_STEPS;
        this->Homing_speed_alpha = 1300;
        this->Homing_acceleration_alpha = 500;

        // this->steps_per_mm_for_z = 200;
        this->steps_per_rad_for_a = 33;
}

void SpringMakerHardwareConfig::PrintOut(){
    Serial.print("\n\n===============================================");
    Serial.print("\nSpringMakerHardwareConfig::PrintOut()");
    Serial.print("\n   STEPPER_DRIVER_MICRO_STEPS = ");
    Serial.print(this->STEPPER_DRIVER_MICRO_STEPS);
    Serial.print("\n   MOTOR_GEARBOX_RATIO_ALPHA_BETA = ");
    Serial.print(this->MOTOR_GEARBOX_RATIO_ALPHA);

    Serial.print("\n    steps_per_rad_for_a = ");
    // Serial.print(this->steps_per_mm_for_z);
    // Serial.print("\n    steps_per_rad_for_w = ");
    Serial.print(this->steps_per_rad_for_a);
    Serial.print("\n     Home_position (a) = (");
    Serial.print(this->Homed_position_a);
    Serial.print(" , ");
    // Serial.print(this->Homed_position_w);
    // Serial.print(" )");

    Serial.print("\n     Homing_speed_alpha = ");
    Serial.print(this->Homing_speed_alpha);
    Serial.print("\n     Homing_acceleration_alpha = ");
    Serial.print(this->Homing_acceleration_alpha);

    Serial.print("\n\n        max_speed_alpha = ");
    Serial.print(max_acceleration_alpha);
    // Serial.print("\n        max_acceleration_alpha_beta = ");
    // Serial.print(max_acceleration_alpha_beta);


    Serial.print("\n----------------------------------------------\n\n");
}
