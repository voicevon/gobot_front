#pragma once
#include "CNC/solution/core_yz/cnc_core_yz_base.h"


class BoxCarrier_CncSolution: public CncSolution_CoreYZBase{
    EnumAxis ConvertToEnum(char axis) override;

};