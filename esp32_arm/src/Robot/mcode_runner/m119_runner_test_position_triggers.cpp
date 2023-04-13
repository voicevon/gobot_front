
#include "m119_runner_test_position_triggers.h"
#include "Robot/axis_homer/position_trigger_array.h"

void M119_Runner_TestPositionTriggers::SetupRunner(GcodeText* mcode){
    
}

// Print all triggers status, when there is a change.
bool M119_Runner_TestPositionTriggers::RunnerLoop(){
    Logger::Debug("M119_Runner::Run()");
    static uint32_t last_flags;
    // HomeTrigger_Array::Instance().GetFiredPosition('X');
    uint32_t flags = gs_PositionTrigger_Array::Instance().GetStateBitsFlag();
    if (flags != last_flags){
        Serial.print(flags,BIN);
        Logger::Print("\t\tflags", flags);

    }
    return true;   
}