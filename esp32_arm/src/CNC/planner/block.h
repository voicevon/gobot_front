#pragma once
#include "CNC/coordinate/cnc_axis.h"


class Block{
    public:
        EnumAxis axis;
        bool IsAbsTargetPosition;
        float TargetPosition;
        float Speed;
        float Acceleration;
    private:


};