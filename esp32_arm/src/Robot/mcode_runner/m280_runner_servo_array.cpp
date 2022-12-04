#include "m280_runner_servo_array.h"


void M280_Runner_Servo::SetupRunner(Gcode* mcode){
    _index = mcode->get_value('P');
    _angle = mcode->get_value('S');
}

// bool M280_Runner_Servo::RunnerLoop(){

//     return true;
// }
