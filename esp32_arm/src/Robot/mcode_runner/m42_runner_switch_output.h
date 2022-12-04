#pragma once
#include "mcode_runner_base.h"

// https://reprap.org/wiki/G-code#M42:_Switch_I.2FO_pin 

class M42_Runner_Switch_Output: public McodeRunnerBase{
    public:
        void SetupRunner(Gcode* mcode) override;

    protected:
        uint8_t _index;
        bool _is_on;
};