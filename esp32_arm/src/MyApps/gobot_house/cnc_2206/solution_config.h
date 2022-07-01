#pragma once

#include "CNC/solution/scara/config_base.h"


class GobotHouse_CncSolutionConfig_2206: public CncSolution_ScaraConfigBase{
    public:
        GobotHouse_CncSolutionConfig_2206();
        // void Init(char solution_id) override;
        void PrintOut(const char * title) override;
        // float GetHomingVelocity(char axis_name) override;

    private:

        void __InitStaticConfig();

};


