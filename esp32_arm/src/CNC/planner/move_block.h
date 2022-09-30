#pragma once
#include "CNC/coordinate/cnc_axis.h"
#include "MyLibs/common_queue/common_queue.h"

class MoveBlock_SingleActuator{
    public:
        EnumAxis_Inverseinematic axis;
        bool IsAbsTargetPosition;
        float TargetPosition;
        float Speed;
        float Acceleration;
};


class MoveBlock: public Queue_able{
    public:
        MoveBlock_SingleActuator MoveBlocks[6];
        void DeepCopyTo(Queue_able* copy) override;
        void DeepReset_ToDefault() override;

    private:


};