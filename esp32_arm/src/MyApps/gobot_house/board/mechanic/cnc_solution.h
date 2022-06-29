#pragma once

#include "CNC/solution/scara/cnc_scara.h"
class GobotHouseCnc: public CncScaraSolution{
    public:
        EnumAxis ConvertToEnum(char axis) override;
    
};