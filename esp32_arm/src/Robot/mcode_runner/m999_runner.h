#pragma once
#include "mcode_runner_base.h"
class M999_Runner: public McodeRunnerBase{
    public:
        void SetupRunner(Gcode* mcode) override;
        bool RunnerLoop() override;
};
