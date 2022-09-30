#include "cnc_axis.h"

        static EnumAxis_ForwardKinematic ForwardKinematic_Axis(char name);
        static EnumAxis_Inverseinematic InverserKinematic_Axis(char name);
EnumAxis_ForwardKinematic CncAxis::ForwardKinematic_Axis(char name){
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

EnumAxis_Inverseinematic CncAxis::InverserKinematic_Axis(char name){
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
            result_axis = AXIS_EPSION;
            break;
        case 'z':
            result_axis = AXIS_ZETA;
            break;
        case 't':
            result_axis = AXIS_THETA;
            break;
        case 'k':
            result_axis = AXIS_KAPPPA;
            break;
        default:
            break;
    }
    return result_axis;
}

