#include "smoker_cnc_mover.h"
#include "actuator_alpha.h"


void Smoker_CncMover::Init(Smoker_Board* board){
    // Smokebot_ActuatorAlphaMechanic config;
    // this->_actuator_alpha_base = board->GetActuator(AXIS_ALPHA);
    
}

void Smoker_CncMover::PrintOut(const char* title){
    Logger::Debug(title);
    Logger::Debug("\t\tVsc_CncMover::PrintOut()");
}
