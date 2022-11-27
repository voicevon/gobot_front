#pragma once
#include "mcode_runner_base.h"
#include "MyLibs/list/pid_controllers_list.h"


// TODO:  Example, How to use M130
class M130_Runner_UpdatePid: public McodeRunnerBase{
    public:
        bool StartExec(Gcode* gcode) override;
    private:
    
};