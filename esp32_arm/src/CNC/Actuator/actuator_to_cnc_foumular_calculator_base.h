#pragma once
#include "stdint.h"
#include "Mylibs/basic/logger.h"


class ActuatorToCncFormular_CalculatorBase{
    public:
        virtual float GetActuatorToCncFormular_Slope_raw_per_mm();
        virtual float GetActuatorToCncFormular_Slope_raw_per_rad();
        
    protected:

    private:

};