#pragma once

#include "Robot/mcode_runner/m123_runner.h"

class GobotMain_M123_Runner: public Final_M123_RunnerBase{
    public:
        // bool StartToRun(GcodeText* gcode) override;
        void SpinOnce() override;

    private:

};