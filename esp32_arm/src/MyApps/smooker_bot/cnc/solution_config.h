#pragma once

#include "CNC/arm_solution/single_axis/kinematic_config.h"

class SmookerBot_CncSolutionConfig: public SingleAxis_KinematicConfig{
    public:
        SmookerBot_CncSolutionConfig();
        // int32_t GetStepsPerUnit(char axis_name) override;
        // void Init(char solution_id) override;
        void PrintOut(const char * title) override;
        
    private:

};