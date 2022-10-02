#pragma once
#include "mcode_runner_base.h"
// #include "MyLibs/pid_controllers/pid_controllers.h"
// #include "MyLibs/pid_controllers/pid_controllers_diction.h"
// #include "MyLibs/dictions/pid_controllers_diction.h"
#include "MyLibs/array/pid_controllers_array.h"

class M130_Runner: public McodeRunnerBase{
    public:
        void Run(Gcode* gcode) override;
        // void LinkPidControllers(PidControllers* pids){this->__pid_controllers_m130=pids;};
    private:
        // PidControllers* __pid_controllers_m130;  
    
};