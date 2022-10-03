#pragma once
#include "mcode_runner_base.h"

#include "MyLibs/list/pid_controllers_list.h"

class Final_M123_RunnerBase{
    public:
        virtual bool StartToRun(Gcode* gcode);
        virtual bool SpinOnce();
        void LinkToRunnerDispacher();

    private:
};

// From point of view of processing task:  this is not a runner, this is a finnal runner agent.
// From point of Mcode runner diapaching:  this is a runner, I can send the job to you.
class M123_Runner: public McodeRunnerBase{
    public:
        bool StartToRun(Gcode* gcode) override;
        bool SpinOnce() override;
        void LinkFinalRunner(Final_M123_RunnerBase* final_runner){this->__final_runner=final_runner;};

    private:
        Final_M123_RunnerBase* __final_runner;
};