#include "cnc_axis.h"


EnumAxis CncAxis::GetFromName(char name){
    EnumAxis result_axis = AXIS_NONE;
    switch (name){
        case 'X':
            result_axis = AXIS_X;
            break;
        case 'Y':
            result_axis = AXIS_Y;
            break;
        case 'Z':
            result_axis = AXIS_Z;
            break;
        case 'A':
            result_axis = AXIS_ALPHA;
            break;
        case 'B':
            result_axis = AXIS_BETA;
            break;
        case 'C':
            result_axis = AXIS_GAMMA;
            break;
        case 'W':
            result_axis = AXIS_NONE;
            break;
        default:
            break;
    }
    return result_axis;
}
