#include "m42_runner_switch_output.h"

void M42_Runner_Switch_Output::SetupRunner(Gcode* mcode){
    uint8_t _index = mcode->get_value('P');
    uint8_t _is_on = (mcode->get_value('S') !=0) ;
}

