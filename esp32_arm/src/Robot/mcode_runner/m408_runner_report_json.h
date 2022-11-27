#pragma once
#include "mcode_runner_base.h"

class M408_Runner_ReportJson: public McodeRunnerBase{
    public:
        bool StartExec(Gcode* mcode) override;
};
