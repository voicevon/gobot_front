#pragma once
#include "CNC/solution/five_bars/cnc_five_bars_base.h"

class GobotMainCnc: public CncFiveBarsBase{
    EnumAxis ConvertToEnum(char axis) override;

};