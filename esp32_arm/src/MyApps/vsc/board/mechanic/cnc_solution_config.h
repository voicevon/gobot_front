#pragma once

// #include "CNC/solution/cnc_single_axis/cnc_single_axis_machine.h"
#include "CNC/solution/cnc_single_axis/cnc_solution_config_single_axis.h"

class VscMachine: public CncSolutionConfig_SingleAxis{
    public:
        VscMachine();
        // void Init(char solution_id) override;
        void PrintOut(const char* title) override;
        
    private:

};