#pragma once

#include "CNC/cnc_core_az/cnc_machine.h"

class BoxMoverCncMachine: public CncCoreAZMachine{
    public:
        void Init(char solution_id) override;
        void PrintOut();
        int32_t GetStepsPerUnit(char axis_name) override;
    private:
};



