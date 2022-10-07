#pragma once

#include "m42_runner.h"
#include "m84_runner.h"
#include "m119_runner.h"
#include "m130_runner.h"

class McodeOS{
    public:
        bool StartToRun(Gcode* gcode);  
        bool SpinOnce();        //todo: Rename RunnerSpinOnce();
        static McodeOS& Instance(){
            static McodeOS __instance; // Guaranteed to be destroyed.
                                  // Instantiated on first use.
            return __instance;
        } 
        McodeRunnerBase* GetRunner(int mcode_id);

    private:
        M42_Runner __m42_runner;
        M84_Runner __m84_runner;
        M119_Runner __m119_runner;
        M130_Runner __m130_runner;
        McodeRunnerBase* __current_runner;
};