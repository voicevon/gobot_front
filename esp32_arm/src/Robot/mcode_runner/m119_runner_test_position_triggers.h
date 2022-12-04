#pragma once
#include "mcode_runner_base.h"

class M119_Runner_TestPositionTriggers: public McodeRunnerBase{
    public:
        void LinkMcode(Gcode* mcode) override;
        bool StartExec() override;
};
