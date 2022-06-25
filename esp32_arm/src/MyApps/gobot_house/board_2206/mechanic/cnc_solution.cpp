#include "cnc_solution.h"

EnumAxis GobotHouse2206Cnc::ConvertToEnum(char axis){
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
            while(true){
                Serial.print("E ");
                delay(500);
            }
            return AXIS_ALPHA;
            break;
    }
}
