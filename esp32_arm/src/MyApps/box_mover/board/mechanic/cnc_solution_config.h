#pragma once

// #include "CNC/solution/core_az/cnc_machine.h"
#include "CNC/solution/core_az/config.h"

class BoxMoverCncMachine: public CncCoreAZMachine{
    public:
        BoxMoverCncMachine();
        // void Init(char solution_id) override;
        void PrintOut();
        // int32_t GetStepsPerUnit(char axis_name) override;
    private:
};



