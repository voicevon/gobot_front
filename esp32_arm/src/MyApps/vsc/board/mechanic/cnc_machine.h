#pragma once

#include "CNC/solution/cnc_single_axis/cnc_single_axis_machine.h"

class VscMachine: public CncSingleAxis_Machine{
    public:
        VscMachine();
        // void Init(char solution_id) override;
        void PrintOut() override;
        
    private:

};