#pragma once

// #include "CNC/solution/cnc_single_axis/cnc_single_axis_machine.h"
#include "CNC/solution/cnc_single_axis/cnc_solution_config_single_axis.h"

class SpringMakerMachine: public CncSolutionConfig_SingleAxis{
    public:
        SpringMakerMachine();
        // int32_t GetStepsPerUnit(char axis_name) override;
        // void Init(char solution_id) override;
        void PrintOut(const char * title) override;
        
    private:

};