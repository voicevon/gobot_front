#include "mover.h"
#include "actuator_alpha.h"


void Vsc_CncMover::Init(Vsc_Board* board){
    Vsc_ActuatorAlphaMechanic config;
    this->_actuator_alpha_base = board->GetActuator(AXIS_ALPHA);
    
}

void Vsc_CncMover::PrintOut(const char* title){
    Serial.println("2222222222222222222222222222222222222222");
    Logger::Debug(title);
    Logger::Debug("\t\tVsc_CncMover::PrintOut()");
}
