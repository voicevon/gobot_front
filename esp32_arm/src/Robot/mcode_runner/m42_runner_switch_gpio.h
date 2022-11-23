#pragma once
#include "mcode_runner_base.h"

// https://reprap.org/wiki/G-code#M42:_Switch_I.2FO_pin 

class M42_Runner_Switch_GPIO: public McodeRunnerBase{
    public:
        bool StartToRun(Gcode* gcode) override;

};