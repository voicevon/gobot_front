#pragma once
#include "CNC/coordinate/cnc_axis.h"
#include "MyLibs/common_queue/common_queue.h"


class MoveBlock: public Queue_able{
    public:
        EnumAxis axis;
        bool IsAbsTargetPosition;
        float TargetPosition;
        float Speed;
        float Acceleration;
        void DeepCopyTo(Queue_able* copy) override;

    private:


};