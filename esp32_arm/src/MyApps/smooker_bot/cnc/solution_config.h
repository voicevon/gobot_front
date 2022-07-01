#pragma once

// #include "CNC/solution/cnc_single_axis/cnc_single_axis_machine.h"
#include "CNC/solution/single_axis/config_base.h"

class SmookerBot_CncMechanic: public CncSolution_SingleAxisConfig{
    public:
        SmookerBot_CncMechanic();
        // int32_t GetStepsPerUnit(char axis_name) override;
        // void Init(char solution_id) override;
        void PrintOut(const char * title) override;
        
    private:

};