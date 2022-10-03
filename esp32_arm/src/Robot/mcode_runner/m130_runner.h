#pragma once
#include "mcode_runner_base.h"
#include "MyLibs/list/pid_controllers_list.h"

class M130_Runner: public McodeRunnerBase{
    public:
        bool StartToRun(Gcode* gcode) override;
    private:
    
};