#pragma once
#include "CNC/solution/cnc_single_axis/cnc_single_axis.h"
class SmookerBot_CncSoution: public CncSingleAxis{
    public:
        EnumAxis ConvertToEnum(char axis) override;



};