#pragma once

#include "CNC/cnc_five_bars/cnc_five_bar_machine.h"

class GobotMainMachine: public CncFiveBarMachine{
    public:
        void Init(char solution_id) override;
        // void Init();
        void PrintOut(const char* title);
        int32_t GetStepsPerUnit(char axis_name) override;
        
    private:
};