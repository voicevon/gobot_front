#pragma once

#include "cnc_scara_config.h"

class GobotHouseCncConfig{

};


class GobotHouseHardwareConfig: public CncScaraConfig{
    public:
        void Init();
        void PrintOut();
        int32_t GetStepsPerUnit(char axis_name) override;
    private:

};


