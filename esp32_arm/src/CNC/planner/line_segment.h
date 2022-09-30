#pragma once
#include "CNC/coordinate/cnc_axis.h"
#include "CNC/coordinate/coordinate_base.h"
#include "MyLibs/common_queue/common_queue.h"


class LineSegment: public Queue_able{
    public:
        EnumAxis_ForwardKinematic axis;
        bool IsAbsTargetPosition;
        FkPositionBase* TargetPosition;
        float Speed;
        float Acceleration;
};

// class Queue_LineSegments: public CommonQueue{
//     public:

// };