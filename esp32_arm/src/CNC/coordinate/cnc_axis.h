#pragma once
#include <stdint.h>

#define CNC_AXIS_COUNT 6

// enum EnumAxis{
//     AXIS_NONE = 0,
//     AXIS_ALPHA = 1, 
//     AXIS_BETA = 2, 
//     AXIS_GAMMA = 3,
//     AXIS_X = 4, 
//     AXIS_Y = 5, 
//     AXIS_Z = 6,
// };

enum EnumAxis_ForwardKinematic{
    AXIS_X = 'X', 
    AXIS_Y = 'Y', 
    AXIS_Z = 'Z',
    AXIS_PITCH = 'A',
    AXIS_ROLL = 'B',
    AXIS_YAW = 'C',
};

enum EnumAxis_Inverseinematic{
    AXIS_ALPHA = 'a', 
    AXIS_BETA = 'b', 
    AXIS_GAMMA = 'g',
    AXIS_DELTA = 'd',
    AXIS_EPSION = 'e',
    AXIS_ZETA = 'z',
    AXIS_THETA = 't',
    AXIS_KAPPPA ='k',
};


class CncAxis{
    public:
        // static EnumAxis_ForwardKinematic ForwardKinematic_Axis(char name);
        // static EnumAxis_Inverseinematic InverserKinematic_Axis(char name);
        // static uint8_t GetIndex(EnumAxis_ForwardKinematic axis);
        // static uint8_t GetIndex(EnumAxis_Inverseinematic axis);
        // static uint8_t GetIndex(char name);

        static uint8_t From_Name_ToIndex(char name);
        static uint8_t From_Enum_ToIndex(EnumAxis_ForwardKinematic axis);
        static uint8_t From_Enum_ToIndex(EnumAxis_Inverseinematic axis);
        static EnumAxis_ForwardKinematic From_FkName_ToEnum(char name);
        static EnumAxis_Inverseinematic From_Ik_Name_ToEnum(char name);

};



