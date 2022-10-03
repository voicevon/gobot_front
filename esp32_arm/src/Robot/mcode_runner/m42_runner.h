#pragma once
#include "mcode_runner_base.h"

class M42_Runner: public McodeRunnerBase{
    public:
        bool StartToRun(Gcode* gcode) override;

};