#include "mcode_runners.h"

void McodeRunners::Run(Gcode* mcode){
    McodeRunnerBase* runner = McodeRunners::getInstance().GetRunner(mcode->m);
    runner->Run(mcode);
}


McodeRunnerBase* McodeRunners::GetRunner(int mcode_id){
    switch(mcode_id){
        case 42:
            return &this->__m42_runner;
            break;
        case 84:
            return &this->__m84_runner;
            break;
        case 130:
            return &this->__m130_runner;
            break;                        
        default:
            break;
    }
}

