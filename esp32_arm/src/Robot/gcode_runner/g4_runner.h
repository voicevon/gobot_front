#pragma once
#include <stdint.h>
#include <Arduino.h>   // for millis()
// #include <wiring_private.h>

#include "sync_gcode_runner_base.h"

class G4_Runner: public SyncGcodeRunnerBase{
    public:
        void SetTarget_in_ms(uint32_t delay_target){this->__delay_target_in_ms=delay_target;};
        void Start() override {this->__start_time_stamp = millis();};
        bool IsDone() override {return (millis() - __start_time_stamp > __delay_target_in_ms);};

    private:
        unsigned long __delay_target_in_ms;
        unsigned long __start_time_stamp;

};