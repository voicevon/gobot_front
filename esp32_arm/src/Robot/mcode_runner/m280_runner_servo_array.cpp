#include "m280_runner_servo_array.h"


void M280_Runner_Servo::SetupRunner(GcodeText* mcode_text){
    GcodeHelper gcode_helper = GcodeHelper(mcode_text->bytes);
    GcodeHelper* mcode = &gcode_helper;
    _index = mcode->get_value('P');
    _angle = mcode->get_value('S');
}

// bool M280_Runner_Servo::RunnerLoop(){

//     return true;
// }
