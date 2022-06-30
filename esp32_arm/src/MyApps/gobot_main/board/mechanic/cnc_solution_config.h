#pragma once

#include "CNC/solution/five_bars/config.h"


class GobotMain_CncSolutionConfig: public CncFiveBarMachine{
    public:
        GobotMain_CncSolutionConfig();
        void PrintOut(const char* title) override;

    private:

        void __InitStaticConfig();

};


