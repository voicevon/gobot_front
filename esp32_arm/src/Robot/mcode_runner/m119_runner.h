#pragma once
#include "mcode_runner_base.h"

class M119_Runner: public McodeRunnerBase{
    public:
        void Run(Gcode* mcode) override;
};
