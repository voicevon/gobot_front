
#include "m119_runner.h"
#include "Robot/axis_homer/home_trigger_array.h"


// Print all triggers status, when there is a change.
bool M119_Runner::StartToRun(Gcode* gcode){
    Logger::Debug("M119_Runner::Run()");
    static uint32_t last_flags;
    HomeTrigger_Array::Instance().GetFiredPosition('X');
    uint32_t flags = HomeTrigger_Array::Instance().GetStateBitsFlag();
    if (flags != last_flags){
        Serial.print(flags,BIN);
        Logger::Print("\t\tflags", flags);

    }
    return true;   
}