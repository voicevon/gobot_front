#pragma once
#include "CNC/gcode/Gcode.h"


class SyncGcodeRunnerBase{
    public:
        void LinkGcode(Gcode* gcode){this->_gcode=gcode;};
        virtual void Start();
        virtual bool IsDone();

    protected:
        Gcode* _gcode;

};