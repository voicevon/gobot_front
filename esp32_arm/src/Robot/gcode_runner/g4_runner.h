#pragma once
#include <stdint.h>
#include <Arduino.h>   // for millis()
#include "sync_gcode_runner_base.h"

class G4_Runner: public SyncGcodeRunnerBase{
    public:
        void LinkGcode(Gcode* gcode) override{__delay_target_in_ms=1000.0f * gcode->get_value('S');};
        void Start() override {this->__start_time_stamp = micros();};
        bool IsDone() override {return (micros() - __start_time_stamp > __delay_target_in_ms);};

    private:
        unsigned long __delay_target_in_ms;
        unsigned long __start_time_stamp;

};