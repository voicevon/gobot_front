#pragma once
#include "mcode_runner_base.h"
class M84_Runner_Disable_All_Steppers: public McodeRunnerBase{
    public:
        void SetupRunner(Gcode* mcode) override;
        bool RunnerLoop() override;
};
