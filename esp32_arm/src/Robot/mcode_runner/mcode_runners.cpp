#include "mcode_runners.h"

bool McodeRunners::StartToRun(Gcode* mcode){
    McodeRunnerBase* runner = McodeRunners::Instance().GetRunner(mcode->m);
    if (runner==nullptr){
        Logger::Error("McodeRunners::StartToRun()");
        Logger::Print("mcode->m", mcode->m);
        Logger::Halt("More ablility, More responsbility!");
    }
    return runner->StartToRun(mcode);
}
bool McodeRunners::SpinOnce(){
    __current_runner->SpinOnce();
}

McodeRunnerBase* McodeRunners::GetRunner(int mcode_id){
    __current_runner = nullptr;
    switch(mcode_id){
        case 42:
            __current_runner = &this->__m42_runner;
            break;
        case 84:
            __current_runner = &this->__m84_runner;
            break;
        case 130:
            __current_runner = &this->__m130_runner;
            break;                        
        default:
            break;
    }
    return __current_runner;
}

