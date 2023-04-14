#include "g4_runner.h"

void G4_Runner::LinkGcode(GcodeText* gcode_text) {
    GcodeHelper gcode_helper = GcodeHelper(gcode_text->GetChars());
    __delay_target_in_ms = 1000.0f * gcode_helper.get_value('S');
}


void G4_Runner::Start(){
    __start_time_stamp = micros() / 1000;
}

bool G4_Runner::IsDone(){
    return (micros() / 1000 - __start_time_stamp > __delay_target_in_ms);
}

