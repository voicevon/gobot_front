#include "cnc_axis.h"

uint8_t CncAxis::From_Enum_ToIndex(EnumAxis_ForwardKinematic axis){
    return 0;
}

uint8_t CncAxis::From_Enum_ToIndex(EnumAxis_Inverseinematic axis){
    return 0;
}

uint8_t CncAxis::From_Name_ToIndex(char name){
    return 0;
}

EnumAxis_ForwardKinematic CncAxis::From_FkName_ToEnum(char name){
    EnumAxis_ForwardKinematic result_axis = AXIS_X;
    switch (name){
        case 'Y':
            result_axis = AXIS_Y;
            break;
        case 'Z':
            result_axis = AXIS_Z;
            break;
        case 'A':
            result_axis = AXIS_PITCH;
            break;
        case 'B':
            result_axis = AXIS_ROLL;
            break;
        case 'C':
            result_axis = AXIS_YAW;
            break;
        default:
            break;
    }
    return result_axis;
}

EnumAxis_Inverseinematic CncAxis::From_Ik_Name_ToEnum(char name){
    EnumAxis_Inverseinematic result_axis = AXIS_ALPHA;
    switch (name){
        case 'b':
            result_axis = AXIS_BETA;
            break;
        case 'g':
            result_axis = AXIS_GAMMA;
            break;
        case 'd':
            result_axis = AXIS_DELTA;
            break;
        case 'e':
            result_axis = AXIS_EPSILON;
            break;
        case 'k':
            result_axis = AXIS_KAPPPA;
            break;
        case 'l':
            result_axis = AXIS_LAMBDA;
            break;
        default:
            break;
    }
    return result_axis;
}

