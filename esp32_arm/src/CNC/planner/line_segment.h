#pragma once
#include "CNC/coordinate/cnc_axis.h"
#include "CNC/coordinate/coordinate_base.h"
#include "MyLibs/common_queue/common_queue.h"


class LineSegment: public Queue_able{
    public:
        EnumAxis_ForwardKinematic axis;
        FKPosition_XYZRPW TargetPosition;
        float Speed_mm_per_second;
        float Acceleration;   // Is this needed ?
        bool IsMiddleKinematicPosition = false;    // A middle kinematic position is borrowing this.
        void Calculate_distance_time(FKPosition_XYZRPW* start_position);

        float Distance_mm;
        float Required_time;
        void DeepCopyTo(LineSegment* the_copy);
        void DeepCopyTo_TargetPosition_fk(FKPosition_XYZRPW* target);
        // void DeepReset_ToDefault() override  {};

        void PrintOUt(const char* title);
    protected:

};

