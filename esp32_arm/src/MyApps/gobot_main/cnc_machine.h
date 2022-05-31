#pragma once

#include "CNC/cnc_five_bars/cnc_machine.h"

class GobotMainMachine: public CncFiveBarMachine{
    public:
        void Init(char solution_id) override;
        void PrintOut(const char* title);
        int32_t GetStepsPerUnit(char axis_name) override;
        
    private:
};