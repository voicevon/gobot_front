#pragma once

#include "CNC/cnc_scara/cnc_scara_config.h"


class GobotHouseHardwareConfig: public CncScaraConfig{
    public:
        void Init();
        void PrintOut();
        int32_t GetStepsPerUnit(char axis_name) override;
    private:

};


