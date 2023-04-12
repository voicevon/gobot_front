#pragma once
#include "CNC/gcode/gcode_text.h"
#include "MyLibs/basic/logger.h"

// TODO: async command, like set temperature and wait async.

class McodeRunnerBase{
    public:
        virtual void SetupRunner(Gcode* mcode);
        // return true: Fininshed running.
        // return false: Not finished running. robot state should be blocked. runner will be run backgroundly.
        virtual bool RunnerLoop();
};