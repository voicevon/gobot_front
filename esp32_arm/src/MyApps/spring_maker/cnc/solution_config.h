#pragma once

// #include "CNC/solution/cnc_single_axis/cnc_single_axis_machine.h"
#include "CNC/solution/single_axis/config_base.h"

class SpringMaker_CncSolutionConfig: public CncSolution_SingleAxisConfig{
    public:
        SpringMaker_CncSolutionConfig();
        // int32_t GetStepsPerUnit(char axis_name) override;
        // void Init(char solution_id) override;
        void PrintOut(const char * title) override;
        
    private:

};