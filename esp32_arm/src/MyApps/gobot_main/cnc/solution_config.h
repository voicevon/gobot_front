#pragma once

#include "CNC/solution/five_bars/config_base.h"


class GobotMain_CncSolutionConfig: public CncFiveBarConfig{
    public:
        GobotMain_CncSolutionConfig();
        void PrintOut(const char* title) override;

    private:

        void __InitStaticConfig();

};


