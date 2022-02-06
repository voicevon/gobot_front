#include "gobot_house_hw_config.h"
#include "HardwareSerial.h"


void GobotHouseHardwareConfig::Init(){
        this->motor_steps_per_round = this->motor_steps_per_round * this->MOTOR_MICRO_STEPS;
        this->Homing_acceleration_alpha = 200 * this->MOTOR_MICRO_STEPS;
        this->Homing_speed_alpha = 200 * this->MOTOR_MICRO_STEPS;
        this->Homing_acceleration_beta = 200 * this->MOTOR_MICRO_STEPS;
        this->Homing_speed_beta = 200 * this->MOTOR_MICRO_STEPS;
}

void GobotHouseHardwareConfig::PrintOut(){
    Serial.print("\n\n===============================================");
    Serial.print("\n    GobotHouseHardwareConfig::PrintOut()");
}
