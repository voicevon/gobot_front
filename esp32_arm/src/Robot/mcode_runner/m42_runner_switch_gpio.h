#pragma once
#include "mcode_runner_base.h"

// https://reprap.org/wiki/G-code#M42:_Switch_I.2FO_pin 

class M42_Runner_Switch_GPIO: public McodeRunnerBase{
    public:
        void SetupRunner(Gcode* mcode) override;
        bool StartExec() override;

    private:
        uint8_t pin_number;
        uint8_t pin_value ;
};