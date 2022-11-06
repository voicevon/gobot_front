#include "mcode_os.h"

bool McodeOS::StartToRun(Gcode* mcode){
    McodeRunnerBase* runner = McodeOS::Instance().GetRunner(mcode->m);
    if (runner==nullptr){
        Logger::Error("McodeOS::StartToRun()");
        Logger::Print("mcode->m", mcode->m);
        Logger::Halt("More ablility, More responsbility!");
    }
    return runner->StartToRun(mcode);
}
bool McodeOS::SpinOnce(){
    __current_runner->SpinOnce();
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
        case 130:
            __current_runner = &this->__m130_runner;
            break;                        
        default:
            break;
    }
    return __current_runner;
}

