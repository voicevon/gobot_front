#pragma once
// #include "CNC/solution/core_az/box_mover_hw.h"
#include "CNC/solution/core_az/cnc_core_az.h"
class BoxMover_CncSoution: public CncCoreAZ{
    public:
        EnumAxis ConvertToEnum(char axis) override;



};