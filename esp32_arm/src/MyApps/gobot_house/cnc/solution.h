#pragma once
#include "CNC/arm_solution/scara/cnc_scara.h"

class GobotHouse_CncSolution: public CncScaraSolution{
    EnumAxis ConvertToEnum(char axis) override;

};