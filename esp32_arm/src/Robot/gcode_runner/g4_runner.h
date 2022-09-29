#pragma once
#include <stdint.h>
#include <Arduino.h>   // for millis()
#include "sync_gcode_runner_base.h"

class G4_Runner: public SyncGcodeRunnerBase{
    public:
        void Start() override {__delay_target_in_ms=1000.0f * _gcode->get_value('S');this->__start_time_stamp = micros();};
        bool IsDone() override {return (micros() - __start_time_stamp > __delay_target_in_ms);};

    private:
        unsigned long __delay_target_in_ms;
        unsigned long __start_time_stamp;

};