#include "m42_runner.h"

void M42_Runner::Run(Gcode* gcode){
    uint8_t pin_number = gcode->get_value('P');
    uint8_t pin_value = gcode->get_value('S');
	digitalWrite(pin_number, pin_value);
    
}