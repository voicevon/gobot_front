#pragma once
#include "stdint.h"
#include "Mylibs/basic/logger.h"


class ActuatorToCncFormular_CalculatorBase{
    public:
        virtual float Get_Formular_Slope_raw_per_mm();
        virtual float Get_Formular_Slope_raw_per_rad();
        
    protected:

    private:

};