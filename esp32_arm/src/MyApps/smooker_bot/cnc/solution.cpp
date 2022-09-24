#include "solution.h"

    //    HomingConfig* config = this->GetAxisHomers()->GetAxisHomer(AXIS_ALPHA)->GetHomingConfig();
    //     config->IsDirectionToMax = true;
    //     // config->LastHomedPosition = 1
    //     config->Accelleration = 6000;
    //     config->DistanceToGo = 100;
EnumAxis SmookerBot_CncSoution::ConvertToEnum(char axis){
    switch (axis){
        case 'A':
            return AXIS_ALPHA;
            break;

        default:
            Serial.print("[Error] SmookerBot_CncSoution::ConvertToEnum() Unknown axis= ");
            Serial.println(axis);
            while(true){
                Serial.print("E ");
                delay(500);
            }
            return AXIS_ALPHA;
            break;
    }
}
