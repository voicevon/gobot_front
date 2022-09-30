#include "m130_runner.h"

void M130_Runner::Run(Gcode* gcode){
    uint8_t p_value = 33;   //TODO: Make sure this is no harmful!
    uint8_t s_value = 0;
    float f_value = 0.0f;

    Logger::Info("M130_Runner::Run()");
    Logger::Print("gcode", gcode->get_command());
    p_value =  gcode->get_value('N');
    Logger::Print("Index", p_value);
    
    f_value = gcode->get_value('P');
    this->__pid_controllers_m130->GetController(p_value)->P = f_value;
    Logger::Print("P", f_value);

    f_value = gcode->get_value('I');
    this->__pid_controllers_m130->GetController(p_value)->I = f_value;
    Logger::Print("I", f_value);

    f_value = gcode->get_value('D');
    this->__pid_controllers_m130->GetController(p_value)->D = f_value;
    Logger::Print("D", f_value);
}
