#pragma once
#include "CNC/arm_solution/single_axis/single_axis_arm_solution.h"

class SpringMaker_ArmSolution: public SingleAxis_ArmSolution{
    public:
        EnumAxis ConvertToEnum(char axis) override;



};