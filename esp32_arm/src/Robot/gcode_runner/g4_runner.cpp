#include "g4_runner.h"

void G4_Runner::LinkGcode(Gcode* gcode) {
    __delay_target_in_ms = 1000.0f * gcode->get_value('S');
}


void G4_Runner::Start(){
    this->__start_time_stamp = micros() / 1000;
}

bool G4_Runner::IsDone(){
    return (micros() / 1000 - __start_time_stamp > __delay_target_in_ms);
}

