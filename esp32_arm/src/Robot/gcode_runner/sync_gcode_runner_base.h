#pragma once
#include "CNC/gcode/gcode_text.h"
#include "CNC/gcode/gcode_helper.h"

class SyncGcodeRunnerBase{
    public:
        virtual void LinkGcode(GcodeText* gcode);
        virtual void Start();
        virtual bool IsDone();

    protected:
        // GcodeText* _gcode;

};