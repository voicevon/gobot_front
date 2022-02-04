#include "box_mover_hw_config.h"
#include "HardwareSerial.h"


void BoxMoverHardwareConfig::Init(){
        this->driver_microsteps = 1;
        this->motor_steps_per_round = 514.0 * this->driver_microsteps;
        this->Homing_acceleration_alpha = 200 * this->driver_microsteps;
        this->Homing_speed_alpha = 200 * this->driver_microsteps;
        this->Homing_acceleration_beta = 200 * this->driver_microsteps;
        this->Homing_speed_beta = 200 * this->driver_microsteps;;
}

void BoxMoverHardwareConfig::PrintOut(){
    Serial.print("\n\n===============================================");
    Serial.print("\n    BoxMoverHardwareConfig::PrintOut()");
}
