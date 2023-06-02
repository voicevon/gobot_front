#pragma once
#include "CNC/gcode/gcode_text.h"
#include "CNC/gcode/gcode_helper.h"
#include "MyLibs/utility/logger.h"

// TODO: async command, like set temperature and wait async.

class McodeRunnerBase{
    public:
        virtual void SetupRunner(GcodeText* mcode);
        // return true: Fininshed running.
        // return false: Not finished running. robot state should be blocked. runner will be run backgroundly.
        virtual bool RunnerLoop();
};