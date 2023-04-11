#pragma once
#include "CNC/coordinate/cnc_axis.h"
#include "CNC/coordinate/coordinate_base.h"
#include "MyLibs/basic/queue/queue_base.h"
#include "move_block_single_actuator.h"


// class MoveBlock: public Queue_able{
class MoveBlock{
    public:
        MoveBlock_SingleActuator MoveBlocks[CNC_ACTUATORS_IDEAL_COUNT];
        // void DeepCopyTo(Queue_able* copy) override;
        void DeepCopyTo(MoveBlock* copy);
        void DeepCopyToIkPosition(IKPosition_abgdekl* to_position);
        // void DeepReset_ToDefault() override;
        void DeepReset_ToDefault();
        void PrintOut(const char* title);
    private:


};