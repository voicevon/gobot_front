#pragma once
#include "CNC/solution/core_az/cnc_core_az.h"

class BoxMover_CncSoution: public CncSolution_CoreAZ{
    public:
        EnumAxis ConvertToEnum(char axis) override;



};