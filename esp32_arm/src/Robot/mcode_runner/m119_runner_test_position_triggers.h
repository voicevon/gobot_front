#pragma once
#include "mcode_runner_base.h"

class M119_Runner_TestPositionTriggers: public McodeRunnerBase{
    public:
        bool StartExec(Gcode* mcode) override;
};
