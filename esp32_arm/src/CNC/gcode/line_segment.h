#pragma once
#include "CNC/coordinate/cnc_axis.h"
#include "CNC/coordinate/coordinate_base.h"


class LineSegment{
    public:
        EnumAxis axis;
        bool IsAbsTargetPosition;
        FkPositionBase* TargetPosition;
        float Speed;
        float Acceleration;
};