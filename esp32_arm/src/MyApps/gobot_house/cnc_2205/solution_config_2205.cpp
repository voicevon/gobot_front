
#include "solution_config_2205.h"


GobotHouse_CncSolotuionConfig_2205::GobotHouse_CncSolotuionConfig_2205(){
        // this->_HOMING_DIR_IS_TO_MAX[AXIS_ALPHA] = 1;
        // this->_HOMING_SPEED[AXIS_ALPHA] = 200.0f;
        
        // this->_HOMING_DIR_IS_TO_MAX[AXIS_BETA] = -1;
        // this->_HOMING_SPEED[AXIS_BETA] = 200.0f;
        // this->_HOMED_POSITION[AXIS_ALPHA] = 13.6;
        // this->_HOMED_POSITION[AXIS_BETA] = 138;
        this->PrintOut("GobotHouse_CncSolotuionConfig_2205");
}




void GobotHouse_CncSolotuionConfig_2205::PrintOut(const char* title){

    Serial.print("\n\n===============================================");
    Serial.print("\n   GobotHouse_CncSolotuionConfig_2205::PrintOut()");
    Serial.print("\n       Homed_position_alpha_in_degree= ");
    // Serial.print(RAD_TO_DEG * this->Homed_position_alpha_in_rad);

    Serial.print("\n       Homed_position_beta_in_degree= "); 
    // Serial.print(RAD_TO_DEG * this->Homed_position_beta_in_rad);


    Serial.print("\n\n===============================================");
}

