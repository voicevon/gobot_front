#pragma once

#include "m42_runner.h"
#include "m84_runner.h"
#include "m130_runner.h"

//todo:  better name:  McodeRunner_Finder
class McodeRunners{
    public:
        bool StartToRun(Gcode* gcode);  //todo: Rename to FindRunnerThenRun
        bool SpinOnce();        //todo: Rename RunnerSpinOnce();
        static McodeRunners& Instance(){
            static McodeRunners __instance; // Guaranteed to be destroyed.
                                  // Instantiated on first use.
            return __instance;
        } 
        McodeRunnerBase* GetRunner(int mcode_id);

    private:
        M42_Runner __m42_runner;
        M84_Runner __m84_runner;
        M130_Runner __m130_runner;
        McodeRunnerBase* __current_runner;
};