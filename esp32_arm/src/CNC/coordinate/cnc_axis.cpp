#include "cnc_axis.h"


bool CncAxis::IsCncAxis_FkName(char name){
    if (name == 'X') return true;
    if (name == 'Y') return true;
    if (name == 'Z') return true;
    if (name == 'U') return true;
    if (name == 'V') return true;
    if (name == 'W') return true;
    return false;
}

bool CncAxis::IsActuator_IkName(char name){
    if (name == 'a') return true;
    if (name == 'b') return true;
    if (name == 'g') return true;
    if (name == 'd') return true;
    if (name == 'e') return true;
    if (name == 'k') return true;
    if (name == 'l') return true;
    return false;
}

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
        case 'U':
            result_axis = AXIS_PITCH;
            break;
        case 'V':
            result_axis = AXIS_ROLL;
            break;
        case 'W':
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

