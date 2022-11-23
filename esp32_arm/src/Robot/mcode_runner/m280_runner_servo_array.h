#pragma once
#include "mcode_runner_base.h"
// #include "MyLibs/list/pid_controllers_list.h"


// TODO:  Example, How to use M130
class M280_Runner_Servo: public McodeRunnerBase{
    public:
        bool StartToRun(Gcode* gcode) override;
    private:
    
};