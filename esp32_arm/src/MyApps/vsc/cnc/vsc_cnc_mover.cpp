#include "vsc_cnc_mover.h"
#include "actuator_alpha.h"


void Vsc_CncMover::Init(Vsc_Board* board){
    Vsc_ActuatorAlphaMechanic config;
    this->_actuator_alpha_base = board->GetActuator(AXIS_ALPHA);
    
    // this->_micro_steps_on_stepper_driver = 16;
    // this->_motor_gear_teeth_count = 10;
    // this->_slave_pulley_teeth_count = 90;
    // this->_motor_step_angle_in_degree = 0.7003891050583658;
}

void Vsc_CncMover::PrintOut(const char* title){
    Logger::Debug(title);
    Logger::Debug("\t\tVsc_CncMover::PrintOut()");
}
