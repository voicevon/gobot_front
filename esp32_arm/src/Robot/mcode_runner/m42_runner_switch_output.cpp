#include "m42_runner_switch_output.h"

void M42_Runner_Switch_Output::SetupRunner(Gcode* mcode){
    _index = mcode->get_value('P');
    _is_on = (mcode->get_value('S') !=0) ;

    // Logger::Print("M42_Runner_Switch_Output::SetupRunner() index", _index);
    // Logger::Print("M42_Runner_Switch_Output::SetupRunner() _is_on", _is_on);
}

