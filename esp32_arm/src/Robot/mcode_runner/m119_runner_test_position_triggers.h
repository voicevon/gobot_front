#pragma once
#include "mcode_runner_base.h"

class M119_Runner_TestPositionTriggers: public McodeRunnerBase{
    public:
        void SetupRunner(GcodeText* mcode) override;
        bool RunnerLoop() override;
};
