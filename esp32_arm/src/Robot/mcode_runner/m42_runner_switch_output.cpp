#include "m42_runner_switch_output.h"

void M42_Runner_Switch_Output::SetupRunner(GcodeText* mcode_text){

    GcodeHelper gcode_helper = GcodeHelper(mcode_text->bytes);
    GcodeHelper* mcode = &gcode_helper;
    _index = mcode->get_value('P');
    _is_on = (mcode->get_value('S') !=0) ;

    // Logger::Print("M42_Runner_Switch_Output::SetupRunner() index", _index);
    // Logger::Print("M42_Runner_Switch_Output::SetupRunner() _is_on", _is_on);
}

