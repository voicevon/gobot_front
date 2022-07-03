#pragma once

#include "CNC/solution/single_axis/config_base.h"

class SmookerBot_CncSolutionConfig: public CncSolution_SingleAxisConfig{
    public:
        SmookerBot_CncSolutionConfig();
        // int32_t GetStepsPerUnit(char axis_name) override;
        // void Init(char solution_id) override;
        void PrintOut(const char * title) override;
        
    private:

};