#include "m130_runner_pid_list.h"

void M130_Runner_UpdatePid::SetupRunner(GcodeText* mcode_text){
    // uint8_t n_value = 33;   //TODO: Make sure this is no harmful!
    // float f_value = 0.0f;
    GcodeHelper mcode = GcodeHelper(mcode_text->GetChars);
    // GcodeHelper* mcode = &gcode_helper;
    Logger::Info("M130_Runner::Run()");
    Logger::Print("gcode", mcode.get_command());
    index =  mcode.get_value('N');
    // PidControllers_Listable* pid = PidControllers_List::Instance().GetPidController(index);
    
    p_value = mcode.get_value('P');
    // pid->P = f_value;

    i_value = mcode.get_value('I');
    // pid->I = f_value;

    d_value = mcode.get_value('D');
    // pid->D = f_value;
}

bool M130_Runner_UpdatePid::RunnerLoop(){
    PidControllers_Listable* pid = PidControllers_List::Instance().GetPidController(index);
    pid->P = p_value;
    pid->I = i_value;
    pid->D = d_value;
    Logger::Print("Index", index);
    Logger::Print("P", p_value);
    Logger::Print("I", i_value);
    Logger::Print("D", d_value);

    return true;
}
