#pragma once
#include <stdint.h>
#include <Arduino.h>   // for millis()
#include "sync_gcode_runner_base.h"

class G4_Runner: public SyncGcodeRunnerBase{
    public:
        void LinkGcode(GcodeText* gcode) override;
        void Start() override;
        bool IsDone() override;

    private:
        unsigned long __delay_target_in_ms;
        unsigned long __start_time_stamp;

};