#pragma once

// #include "CNC/solution/cnc_single_axis/cnc_single_axis_machine.h"
#include "CNC/solution/single_axis/kinematic_config.h"

class SpringMaker_CncSolutionConfig: public SingleAxis_KinematicConfig{
    public:
        SpringMaker_CncSolutionConfig();
        // int32_t GetStepsPerUnit(char axis_name) override;
        // void Init(char solution_id) override;
        void PrintOut(const char * title) override;
        
    private:

};