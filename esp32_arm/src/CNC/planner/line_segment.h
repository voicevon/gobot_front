#pragma once
#include "CNC/coordinate/cnc_axis.h"
#include "CNC/coordinate/coordinate_base.h"
#include "MyLibs/common_queue/common_queue.h"


class LineSegment: public Queue_able{
    public:
        EnumAxis_ForwardKinematic axis;
        // bool IsAbsTargetPosition;
        // FKPosition_XYZRPY* TargetPosition;
        FKPosition_XYZRPY TargetPosition;
        float Speed;
        float Acceleration;   // Is this needed ?
        void Calculate_distance_time(FKPosition_XYZRPY* start_position);

        float vector_distance;
        float required_time;
        void DeepCopyTo(LineSegment* the_copy);
        void DeepCopyFromFkPosition(FKPosition_XYZRPY* from);

        void PrintOUt();
    protected:

};

// class Queue_LineSegments: public CommonQueue{
//     public:

// };