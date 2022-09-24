#pragma once

#include "axis_homer.h"
#include "CNC/coordinate/cnc_axis.h"
class AxisHomers{
    public:
        AxisHomers(uint8_t axis_count){};
        AxisHomer* GetAxisHomer(EnumAxis axis){return &this->__all_homers[axis];};


    private:
        AxisHomer __all_homers[CNC_AXIS_COUNT];
        uint8_t __axis_count;

};