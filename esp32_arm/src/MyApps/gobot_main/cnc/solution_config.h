#pragma once

#include "CNC/arm_solution/five_bars/config_base.h"


class GobotMain_CncSolutionConfig: public CncSolution_FiveBarConfig{
    public:
        GobotMain_CncSolutionConfig();
        void PrintOut(const char* title) override;

    private:

        void __InitStaticConfig();

};


