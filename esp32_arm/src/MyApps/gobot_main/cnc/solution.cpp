#include "solution.h"

EnumAxis GobotMainCnc::ConvertToEnum(char axis){
    switch (axis){
        case 'A':
            return AXIS_ALPHA;
            break;
        case 'B':
            return AXIS_BETA;
            break;
        case 'X':
            return AXIS_X;
            break;
        case 'Y':
            return AXIS_Y;
            break;
        default:
            Serial.print("[Error] GobotMainCnc::ConvertToEnum() Unknown axis= ");
            Serial.println(axis);
            break;
    }
    return AXIS_ALPHA;
}
