#pragma once
#include "CNC/gcode/gcode_text.h"


class SyncGcodeRunnerBase{
    public:
        virtual void LinkGcode(GcodeText* gcode);
        virtual void Start();
        virtual bool IsDone();

    protected:
        // Gcode* _gcode;

};