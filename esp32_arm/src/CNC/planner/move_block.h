#pragma once
#include "CNC/coordinate/cnc_axis.h"
#include "CNC/coordinate/coordinate_base.h"
#include "MyLibs/common_queue/common_queue.h"
#include "move_block_single_actuator.h"


class MoveBlock: public Queue_able{
    public:
        MoveBlock_SingleActuator MoveBlocks[CNC_ACTUATORS_IDEAL_COUNT];
        // void DeepCopyTo(Queue_able* copy) override;
        void DeepCopyTo(MoveBlock* copy);
        void DeepCopyToIkPosition(IKPosition_abgdekl* to_position);
        void DeepReset_ToDefault() override;
        void PrintOut(const char* title);
    private:


};