#include "spring_maker_robot.h"
#include "Robot/mcode_runner/mcode_os.h"
// #include "Robot/mcode_runner/mcode_runners.h"
#include "CNC/Actuator/stepper/actuator_stepper_calculator.h"

void SpringMakerRobot::Init(Board_SpringMaker* board){
    Logger::Debug("SpringMakerRobot::Init()");
    // this->_cnc_board = board;
    // this->_LinkEef(board->GetEef());

    Queue_MoveBlock::Instance()._all_queue_ables = (Queue_able*)this->__all_move_blocks;
    // this->__planner.__arm_solution = &arm_solution;
    // this->_arm_solution = &this->arm_solution;  

    this->_g28_runner = &this->g28_runner;
    g28_runner.Init(&mover, &arm_solution);

    Logger::Info("SpringMakerRobot::Init() Actuators.");
    CncActuator_List::Instance().Init(__all_actuators, CNC_ACTUATORS_COUNT);
    CncActuator_List::Instance().AddActuator(&__actuator_alpha);
    // this->__actuator_alpha.LinkPidController(&__speed_pid);
    // this->__actuator_alpha.LinkMotorDriver(board->GetMotorDriver());
    // this->__actuator_alpha.LinkAngleSensor(board->GetAngleSensor());
    this->__actuator_alpha.MyName = 'A';
    this->__actuator_alpha.LinkStepper(board->GetStepper(), 1.0f);
    
    // ActuatorStepper_Calculator helper;
    // helper.motor_driver_micro_steps = 10;
    // helper.motor_gear_teeth_count = 12;
    // helper.slave_pulley_teeth_count = 33;
    // float slope = helper.Get_Formular_Slope_steps_per_mm();
    // float slope = 1;
    // this->__actuator_alpha.Formular_SetSlope(slope);
    
    this->_LinkMover(&mover);

}

void SpringMakerRobot::_InitStatic_Queues() {

}



void SpringMakerRobot::_Init_ArmSolution() {


}
