#include "box_carrier_hw_config.h"
#include "HardwareSerial.h"


void BoxCarrierHardwareConfig::Init(){
        this->STEPPER_DRIVER_MICRO_STEPS = 16;
        this->MOTOR_GEARBOX_RATIO_ALPHA_BETA = 27;
        this->MOTOR_DEGREE_PER_STEP = 1.8;

        this->steps_per_motor_round = 360.0f / this->MOTOR_DEGREE_PER_STEP * this->STEPPER_DRIVER_MICRO_STEPS;
        this->steps_per_mm_for_z = 200;
        this->steps_per_mm_for_y = 200;

        this->Homing_speed_alpha_beta = 1300;
        this->Homing_acceleration_alpha_beta = 500;

        this->max_acceleration_alpha_beta = 6000;
        this->max_speed_alpha_beta = 20000;

}

void BoxCarrierHardwareConfig::PrintOut(){
    Serial.print("\n\n===============================================");
    Serial.print("\nBoxMoverHardwareConfig::PrintOut()");
    Serial.print("\n   STEPPER_DRIVER_MICRO_STEPS = ");
    Serial.print(this->STEPPER_DRIVER_MICRO_STEPS);
    Serial.print("\n   MOTOR_GEARBOX_RATIO_ALPHA_BETA = ");
    Serial.print(this->MOTOR_GEARBOX_RATIO_ALPHA_BETA);

    Serial.print("\n    steps_per_mm_for_z = ");
    Serial.print(this->steps_per_mm_for_z);
    Serial.print("\n    steps_per_rad_for_y = ");
    Serial.print(this->steps_per_mm_for_y);
    Serial.print("\n     Home_position (z,y) = (");
    Serial.print(this->Homed_position_z);
    Serial.print(" , ");
    Serial.print(this->Homed_position_y);
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


int32_t BoxCarrierHardwareConfig::GetStepsPerUnit(char axis_name) {
    return 123;
}
