#include "mover.h"
#include "actuator_alpha.h"


void Vsc_CncMover::Init(Vsc_Board* board){
    Vsc_ActuatorAlphaMechanic config;
    this->_actuator_alpha_base = board->GetActuator(AXIS_ALPHA);

}
