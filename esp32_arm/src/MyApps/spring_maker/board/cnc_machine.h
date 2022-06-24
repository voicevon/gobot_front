#pragma once

#include "CNC/solution/cnc_single_axis/cnc_single_axis_machine.h"

class SpringMakerMachine: public CncSingleAxis_Machine{
    public:
        // int32_t GetStepsPerUnit(char axis_name) override;
        void Init(char solution_id) override;
        void PrintOut() override;
        
    private:

};