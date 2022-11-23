#pragma once
#include "CNC/coordinate/cnc_axis.h"
#include "CNC/coordinate/coordinate_base.h"
#include "MyLibs/common_queue/common_queue.h"


class LineSegment: public Queue_able{
    public:
        EnumAxis_ForwardKinematic axis;
        bool IsAbsTargetPosition;
        FKPosition_XYZRPY * TargetPosition;
        float Speed;
        float Acceleration;
        void DeepCopyTo(LineSegment* the_copy);

    protected:

};

// class Queue_LineSegments: public CommonQueue{
//     public:

// };