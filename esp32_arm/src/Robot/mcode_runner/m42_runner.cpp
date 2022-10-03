#include "m42_runner.h"


// NOTICE: output pin should be initialized in Board::Init()
bool M42_Runner::StartToRun(Gcode* gcode){
    uint8_t pin_number = gcode->get_value('P');
    uint8_t pin_value = gcode->get_value('S');
	digitalWrite(pin_number, pin_value);
    return true;
}