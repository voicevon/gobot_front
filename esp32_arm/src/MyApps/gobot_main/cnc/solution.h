#pragma once
#include "CNC/solution/five_bars/cnc_five_bars.h"

class GobotMainCnc: public CncFiveBars{
    EnumAxis ConvertToEnum(char axis) override;

};