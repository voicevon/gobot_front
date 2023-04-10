#pragma once
#include "mcode_runner_base.h"



// TODO:  Example, How to use M130
class M280_Runner_Servo: public McodeRunnerBase{
    public:
        void SetupRunner(Gcode* mcode) override;
        // bool RunnerLoop() override;

    protected:
        int _index;
        int _angle;
};