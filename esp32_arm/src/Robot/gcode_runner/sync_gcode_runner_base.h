#pragma once
#include "CNC/gcode/Gcode.h"


class SyncGcodeRunnerBase{
    public:
        virtual void LinkGcode(Gcode* gcode);
        virtual void Start();
        virtual bool IsDone();

    protected:
        // Gcode* _gcode;

};