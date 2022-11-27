#include "mcode_os.h"

// return true:  running is finished ,
// return false: running is on the go, will keep a long time, like test_position_trigger 99 times.
bool McodeOS::StartToRun(Gcode* mcode){
    McodeRunnerBase* runner = McodeOS::Instance().GetRunner(mcode->m);
    if (runner==nullptr){
        Logger::Error("McodeOS::StartToRun()");
        Logger::Print("mcode->m", mcode->m);
        Logger::Halt("More ablility, More responsbility!");
    }
    return runner->StartExec(mcode);
}
bool McodeOS::SpinOnce(){
    __current_runner->LoopExec();
}

McodeRunnerBase* McodeOS::GetRunner(int mcode_id){
    __current_runner = nullptr;
    switch(mcode_id){
        case 42:
            __current_runner = &this->__m42_runner_switch_gpio;
            break;
        case 84:
            __current_runner = &this->__m84_runner_disable_all_steppers;
            break;
        case 119:
            __current_runner = &this->__m119_runner_test_position_triggers;
            break;
        case 130:
            __current_runner = &this->__m130_runner_update_pid;
            break;  
        case 280:
            __current_runner = &this->__m280_runner_set_servo;
        case 408:
            __current_runner = &this->__m408_runner_report_json;
            break;
        default:
            break;
    }
    return __current_runner;
}

