#pragma once


// #include "single_axis_homer.h"
#include "Robot/position_trigger/single_position_trigger.h"


class PositionTriggers{
    public:

        PositionTriggers(uint8_t triger_count);
        void AppendSingleHomer(SinglePositionTriger* single_homer);

        //return -1, if no homer is trigered.
        uint8_t GetTrigeredIndex();

    private:
        uint8_t __triger_count;
        uint8_t __appending_index;

        SinglePositionTriger* _all_single_homers[4];


};
