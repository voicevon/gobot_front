#pragma once

#include "m42_runner.h"
#include "m84_runner.h"
#include "m130_runner.h"

// class AllRunners{
//     public:
//         M42_Runner __m42_runner;
// };

class McodeRunners{
    public:
        static void Run(Gcode* gcode);
        static McodeRunners& getInstance(){
            static McodeRunners instance; // Guaranteed to be destroyed.
                                  // Instantiated on first use.
            return instance;
        } 
        // static M42_Runner* GetM42(){
        //     return &__m42_runner;
        // }
        McodeRunnerBase* GetRunner(int mcode_id);

    private:
        M42_Runner __m42_runner;
        M84_Runner __m84_runner;
        M130_Runner __m130_runner;
};