#include "m42_runner_switch_gpio.h"

void M42_Runner_Switch_GPIO::SetupRunner(Gcode* mcode){
    uint8_t pin_number = mcode->get_value('P');
    uint8_t pin_value = mcode->get_value('S');
}

// NOTICE: output pin should be initialized in Board::Init()
bool M42_Runner_Switch_GPIO::RunnerLoop(){

	digitalWrite(pin_number, pin_value);
    return true;
}