#pragma once
#include "CNC/solution/scara/cnc_scara.h"

class GobotHouse2206Cnc: public CncScaraSolution{
    EnumAxis ConvertToEnum(char axis) override;

};