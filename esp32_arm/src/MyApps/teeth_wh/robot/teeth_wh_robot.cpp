#include "teeth_wh_robot.h"
#include "Robot/mcode_runner/mcode_os.h"
#include "CNC/Actuator/dc_motor/actuator_encoder_calculator.h"

void TeechWarehouse_Robot::Init(TeethWarehouse_Board* board){
    Logger::Debug("TeechWarehouse_Robot::Init()");
    this->_cnc_board = board;
    // this->_LinkEef(board->GetEef());

    Queue_MoveBlock::Instance()._all_queue_ables = (Queue_able*)this->__all_move_blocks;
    this->__planner.__arm_solution = &arm_solution;
    this->_arm_solution = &this->arm_solution;  
    arm_config.arm_length = 205;
    arm_config.slave_gear_circle_length = 123.98;
    arm_config.master_slope_steps_per_mm = 12.778;
    this->arm_solution.LinkConfig(&arm_config);

    this->__g28_runner = &this->g28_runner;
    g28_runner.Init(&mover);

    this->LinkMover(&mover);
    this->__Init_actuators(board);

}

void TeechWarehouse_Robot::__Init_actuators(TeethWarehouse_Board* board){
    Logger::Info("TeechWarehouse_Robot::Init() Actuators.");
    CncActuator_List::Instance().Init(__all_actuators, CNC_ACTUATORS_COUNT);
    CncActuator_List::Instance().AddActuator(&__actuator_alpha);

    this->__actuator_alpha.MyName = 'a';
    this->__actuator_beta.MyName = 'b';
    
    ActuatorStepper_Calculator helper;
    helper._motor_gear_pitch_in_mm = 12.7;
    helper._slave_pulley_teeth_count = 368;
    float slope = helper.Get_steps_per_mm();
    __actuator_alpha.Init_FomularSlope(slope);


}

void TeechWarehouse_Robot::StoreToCell(int row_index, int col_index){

}

void TeechWarehouse_Robot::RetrieveFromCell(int row_index, int col_index){

}

void TeechWarehouse_Robot::__MoveToCell(int row_index, int col_index){
    // String m123 = "M123C";
    // m123.concat(cell_index);
    // this->_gcode_queue->AppendGcodeCommand(m123);

    String g1 = "G1A";
    // TODO:  over single circle.
    // float gear_angle = TWO_PI *  this->__position_in_pitch[layer_index] / 184;
    // g1.concat(gear_angle);
    g1.concat("F0.1");
    this->_gcode_queue->AppendGcodeCommand(g1);
}