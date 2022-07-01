#pragma once

#include "CNC/solution/single_axis/config_base.h"

class Vsc_CncSolutionConfig: public CncSolution_SingleAxisConfig{
    public:
        Vsc_CncSolutionConfig();
        // void Init(char solution_id) override;
        void PrintOut(const char* title) override;
        
    private:

};