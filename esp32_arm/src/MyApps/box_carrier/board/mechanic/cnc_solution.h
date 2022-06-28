#pragma once
#include "CNC/solution/cnc_core_yz/cnc_core_yz.h"


class BoxCarrier_CncSolution: public Cnc_CoreYZ{
    EnumAxis ConvertToEnum(char axis) override;

};