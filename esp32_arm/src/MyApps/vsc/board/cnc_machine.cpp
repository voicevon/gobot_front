#include "cnc_machine.h"



void VscMachine::Init(char solution){
        this->max_acceleration_alpha = 200;
        this->max_speed_alpha = 200;

        this->Homing_speed_alpha = 100;
        this->Homing_acceleration_alpha = 200;
        this->Home_is_to_max_position = 1;

}

void VscMachine::PrintOut(){
    Serial.print("\n\n===============================================");
    Serial.print("\nSpringMakerMachine::PrintOut()");
    Serial.print("\n     Home_position (a) = (");
    Serial.print(this->Homed_position_fk);
    Serial.print(" , ");

    Serial.print("\n     Homing_speed_alpha = ");
    Serial.print(this->Homing_speed_alpha);
    Serial.print("\n     Homing_acceleration_alpha = ");
    Serial.print(this->Homing_acceleration_alpha);

    Serial.print("\n\n   max_speed_alpha = ");
    Serial.print(max_acceleration_alpha);


    Serial.print("\n----------------------------------------------\n\n");
}

