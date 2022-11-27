#pragma once
#include "mcode_runner_base.h"

class M408_Runner_ReportJson: public McodeRunnerBase{
    public:
        bool StartToRun(Gcode* mcode) override;
};
