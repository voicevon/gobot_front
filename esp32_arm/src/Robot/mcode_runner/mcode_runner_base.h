#pragma once
#include "CNC/gcode/Gcode.h"
#include "MyLibs/basic/logger.h"

// TODO: async command, like set temperature and wait async.

class McodeRunnerBase{
    public:
        // return true: Fininshed running.
        // return false: Not finished running. robot state should be blocked. runner will be run backgroundly.
        virtual bool StartExec(Gcode* mcode);
        virtual bool LoopExec(){return true;};
};