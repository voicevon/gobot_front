#pragma once
#include "mcode_runner_base.h"
#include "MyLibs/pid_controllers/pid_controllers.h"

class M130_Runner: public McodeRunnerBase{
    public:
        void Run(Gcode* gcode) override;
        void LinkPidControllers(PidControllers* pids){this->__pid_controllers_m130=pids;};
    private:
        PidControllers* __pid_controllers_m130;  
    
};