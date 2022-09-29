#include "spring_maker_arm_solution.h"

    // HomingConfig* config = this->GetAxisHomers()->GetAxisHomer(AXIS_ALPHA)->GetHomingConfig();
    // config->IsDirectionToMax = true;
    // config->Accelleration = 6000;
    // config->Speed = 13000.0f;


EnumAxis SpringMaker_ArmSolution::ConvertToEnum(char axis){
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
