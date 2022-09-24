#include "solution.h"

void SpringMaker_CncSoution::Init(){
    HomingConfig* config = this->_homer.GetAxisHomer(AXIS_ALPHA)->GetHomingConfig();
    config->IsDirectionToMax = true;
    config->Accelleration = 6000;
    config->Speed = 13000.0f;
        // this->_HOMING_SPEED[AXIS_ALPHA] = 13000.0f;
        // this->_HOMING_DIR_IS_TO_MAX[AXIS_ALPHA] = 1;
        // this->_HOMING_ACCELERATION[AXIS_ALPHA] = 6000;
}


EnumAxis SpringMaker_CncSoution::ConvertToEnum(char axis){
    switch (axis){
        case 'A':
            return AXIS_ALPHA;
            break;

        default:
            Serial.print("[Error] GobotMainCnc::ConvertToEnum() Unknown axis= ");
            Serial.println(axis);
            while(true){
                Serial.print("E ");
                delay(500);
            }
            return AXIS_ALPHA;
            break;
    }
}
