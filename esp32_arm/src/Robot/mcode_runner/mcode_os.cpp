#include "mcode_os.h"

void McodeOS::SetupRunner(Gcode* mcode){
    __current_runner = McodeOS::Instance().GetRunner(mcode->m);
    if (__current_runner==nullptr){
        Logger::Error("McodeOS::SetupRunner()");
        Logger::Print("mcode->m", mcode->m);
        Logger::Halt("More ability, More responsbility!");
    }
    __current_runner->SetupRunner(mcode);
    // Logger::Info(" McodeOS::SetupRunner() ");
    // Logger::Print("Mcode ", mcode->m);
}

// return true:  running is finished ,
// return false: running is on the go, will keep a long time, like test_position_trigger 99 times.
bool McodeOS::RunnerLoop(){
    return __current_runner->RunnerLoop();
}

McodeRunnerBase* McodeOS::GetRunner(int mcode_id){
    __current_runner = nullptr;
    switch(mcode_id){
        case 42:
            __current_runner = this->__m42_runner_switch_output;
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
            __current_runner = this->__m280_runner_set_servo;
            break;
        case 408:
            __current_runner = &this->__m408_runner_mqtt_publish;
            break;
        case 999:
            __current_runner = &this->__m999_runner;
            break;

        default:
            break;
    }
    return __current_runner;
}

