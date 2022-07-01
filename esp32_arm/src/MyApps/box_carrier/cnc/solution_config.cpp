#include "solution_config.h"
#include "HardwareSerial.h"


void BoxCarrier_CncSolutionConfig::__Init_Common(){
    this->_IS_COMBINED_FK = true;
}

void BoxCarrier_CncSolutionConfig::__Init_Fast(){
        this->Homing_speed_alpha_beta = 6000;
        this->Homing_acceleration_alpha_beta = 5000;
        this->Homed_position_y = 0;
        this->Homed_position_z = 0;

        this->max_acceleration_alpha_beta = 6000;
        this->max_speed_alpha_beta = 20000;
}

void BoxCarrier_CncSolutionConfig::PrintOut(const char* title){
    Serial.print("\n\n===============================================");
    Serial.print("\n   BoxMoverHardwareConfig::PrintOut()");
    
    Serial.print("\n          Home_position (z,y) = (");
    Serial.print(this->Homed_position_z);
    Serial.print(" , ");
    Serial.print(this->Homed_position_y);
    Serial.print(" )");

    Serial.print("\n          Homing_speed_alpha_beta = ");
    Serial.print(this->Homing_speed_alpha_beta);
    Serial.print("\n          Homing_acceleration_alpha_beta = ");
    Serial.print(this->Homing_acceleration_alpha_beta);

    Serial.print("\n\n    max_speed_alpha_beta = ");
    Serial.print(max_acceleration_alpha_beta);
    Serial.print("\n      max_acceleration_alpha_beta = ");
    Serial.print(max_acceleration_alpha_beta);


    Serial.print("\n----------------------------------------------\n\n");
}

