#pragma once
#include "mcode_runner_base.h"
#include "MyLibs/basic/list/pid_controllers_list.h"


// TODO:  Example, How to use M130
class M130_Runner_UpdatePid: public McodeRunnerBase{
    public:
        void SetupRunner(GcodeText* mcode) override;
        bool RunnerLoop() override;

    private:
        int index;
        float p_value;
        float i_value;
        float d_value;

};