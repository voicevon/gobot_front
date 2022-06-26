#pragma once

#include "CNC/solution/cnc_single_axis/cnc_single_axis_machine.h"

class SmookerBot_CncMechanic: public CncSingleAxis_Machine{
    public:
        SmookerBot_CncMechanic();
        // int32_t GetStepsPerUnit(char axis_name) override;
        // void Init(char solution_id) override;
        void PrintOut(const char * title) override;
        
    private:

};