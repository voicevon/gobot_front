#pragma once

#include "axis_homer.h"
#include "CNC/coordinate/cnc_axis.h"

class HomerDiction{  // HomerCollects? HomerDiction? 
    public:
        HomerDiction(uint8_t axis_count){};
        // void Init(int axis_count){this->__axis_count=axis_count;}
        AxisHomer* GetAxisHomer(EnumAxis_ForwardKinematic axis);
        AxisHomer* GetAxisHomer(EnumAxis_Inverseinematic axis);


    private:
        AxisHomer __all_homers[CNC_AXIS_COUNT];
        uint8_t __axis_count;

};