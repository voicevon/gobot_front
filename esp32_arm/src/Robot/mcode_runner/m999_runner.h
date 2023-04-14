#pragma once
#include "mcode_runner_base.h"
class M999_Runner: public McodeRunnerBase{
    public:
        void SetupRunner(GcodeText* mcode) override;
        bool RunnerLoop() override;
};
