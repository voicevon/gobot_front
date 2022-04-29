#include "gobot_house_hw_config.h"
#include "HardwareSerial.h"


void GobotHouseHardwareConfig::Init(){
        // [when microstep==1] The value 514.0 is measured/Calibrated from running 10 rounds, Not calculated from anywhere.
        // This value should be calculated from motor's datasheet.  But can not get datasheet for 28-BGY motor.
        this->motor_steps_per_round = 514 * this->MOTOR_MICRO_STEPS;

        this->STEPS_PER_RAD_ALPHA = this->motor_steps_per_round * this->GEAR_BOX_RATIO_ALPHA / TWO_PI;  
        this->STEPS_PER_RAD_BETA =  this->motor_steps_per_round * this->GEAR_BOX_RATIO_BETA / TWO_PI; 
        this->MAX_ACCELERATION_ALPHPA = 600 * this->MOTOR_MICRO_STEPS;
        this->MAX_STEPS_PER_SECOND_ALPHA = 5000 * this->MOTOR_MICRO_STEPS;    // tested 1200          
        this->MAX_ACCELERATION_BETA = 200 * this->MOTOR_MICRO_STEPS;
        this->MAX_STEPS_PER_SECOND_BETA =  500 * this->MOTOR_MICRO_STEPS; 

        this->Homing_acceleration_alpha = 200 * this->MOTOR_MICRO_STEPS;
        this->Homing_speed_alpha = 200 * this->MOTOR_MICRO_STEPS;   //tested 200 * 1
        this->Homing_acceleration_beta = 500 * this->MOTOR_MICRO_STEPS;
        this->Homing_speed_beta = 60 * this->MOTOR_MICRO_STEPS; 

        this->EEF_Suck_Angle = 164;   // Max is 4096
        this->EEF_Release_Angle = 355;   // Max is 4096

        this->Homed_position_alpha_in_degree = 13.6;
        this->PrintOut();
}

void GobotHouseHardwareConfig::PrintOut(){
    Serial.print("\n\n===============================================");
    Serial.print("\n   GobotHouseHardwareConfig::PrintOut()");
    Serial.print("\n       Homed_position_alpha_in_degree= ");
    Serial.print(this->Homed_position_alpha_in_degree);
    Serial.print("\n       Homed_position_beta_in_degree= "); 
    Serial.print(this->Homed_position_beta_in_degree);

    Serial.print("\n   MOTOR_MICRO_STEPS= ");
    Serial.print(this->MOTOR_MICRO_STEPS);
    Serial.print("\n       motor_steps_per_round=514* MOTOR_MICRO_STEPS= ");
    Serial.print(this->motor_steps_per_round);
    Serial.print("\n   GEAR_BOX_RATIO_ALPHA= ");
    Serial.print(this->GEAR_BOX_RATIO_ALPHA);
    Serial.print("\n   STEPS_PER_RAD_ALPHA= ");
    Serial.print(this->STEPS_PER_RAD_ALPHA);
    Serial.print("\n   MAX_STEPS_PER_SECOND_ALPHA= ");
    Serial.print(this->MAX_STEPS_PER_SECOND_ALPHA);
    Serial.print("\n   MAX_ACCELERATION_ALPHPA= ");
    Serial.print(this->MAX_ACCELERATION_ALPHPA);

    Serial.print("\n\n       STEPS_PER_RAD_BETA= ");
    Serial.print(this->STEPS_PER_RAD_BETA);
    Serial.print("\n\n===============================================");
}
