#pragma once

#include "CNC/cnc_five_bars/cnc_five_bars_config.h"

class GobotChessboardHardwareConfig: public CncFiveBarConfig{
    public:
        void Init();
        void PrintOut(const char* title);
        int32_t GetStepsPerUnit(char axis_name) override;
        
    private:
};