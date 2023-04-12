#include "spring_maker_robot.h"
#include "Robot/mcode_runner/mcode_os.h"
// #include "CNC/Actuator/stepper/actuator_stepper_calculator.h"

void SpringMakerRobot::Init(Board_SpringMaker* board){
    Logger::Debug("SpringMakerRobot::Init()");

    gs_MoveBlock_Queue::Instance()._all_elements = this->__all_move_blocks;

    this->_g28_runner = &this->g28_runner;
    g28_runner.Init(&mover, &arm_solution,"");

    Logger::Info("SpringMakerRobot::Init() Actuators.");
    CncActuator_List::Instance().Init(__all_actuators, CNC_ACTUATORS_COUNT);
    CncActuator_List::Instance().AddActuator(&__actuator_alpha);
    this->__actuator_alpha.MyName = 'A';
    this->_LinkMover(&mover);

}

void SpringMakerRobot::_InitStatic_Queues() {

}



void SpringMakerRobot::_Init_ArmSolution() {


}
