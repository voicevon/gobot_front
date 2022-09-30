#pragma once
#include "CNC/gcode/Gcode.h"
#include "MyLibs/basic/logger.h"

// TODO: async command, like set temperature and wait async.

class McodeRunnerBase{
    public:
        virtual void Run(Gcode* mcode);
};