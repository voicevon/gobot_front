#pragma once

#define CNC_AXIS_COUNT 6

enum EnumAxis{
    AXIS_NONE = 0,
    AXIS_ALPHA = 1, 
    AXIS_BETA = 2, 
    AXIS_GAMMA = 3,
    AXIS_X = 4, 
    AXIS_Y = 5, 
    AXIS_Z = 6,
};

class CncAxis{
    public:
        static EnumAxis GetFromName(char name);
};



