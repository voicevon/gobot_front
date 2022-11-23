#include "teeth_wh_robot.h"
#include "Robot/mcode_runner/mcode_os.h"
#include "CNC/Actuator/dc_motor/actuator_encoder_calculator.h"



void TeechWarehouse_Robot::Init(TeethWarehouse_Board* board){
    Logger::Debug("TeechWarehouse_Robot::Init()");
    this->_cnc_board = board;
    this->__board = board;

    Queue_MoveBlock::Instance()._all_queue_ables = (Queue_able*)this->__all_move_blocks;
    // this->__planner.__arm_solution = &arm_solution;
    this->_arm_solution = &this->arm_solution;  
    arm_config.arm_length = 205;
    arm_config.slave_gear_circle_length = 123.98;
    arm_config.master_slope_steps_per_mm = 12.778;
    this->arm_solution.LinkConfig(&arm_config);

    this->_g28_runner = &this->g28_runner;
    g28_runner.Init(&mover);

    this->_LinkMover(&mover);
    this->__Init_actuators(board);

}

void TeechWarehouse_Robot::__Init_actuators(TeethWarehouse_Board* board){
    Logger::Info("TeechWarehouse_Robot::Init() Actuators.");
    CncActuator_List::Instance().Init(__all_actuators, CNC_ACTUATORS_COUNT);
    CncActuator_List::Instance().AddActuator(&__actuator_alpha);
    CncActuator_List::Instance().AddActuator(&__actuator_beta);
    CncActuator_List::Instance().AddActuator(&__actuator_gamma);

    this->__actuator_alpha.MyName = 'a';
    this->__actuator_beta.MyName = 'b';
    this->__actuator_beta.MyName = 'g';
    
    ActuatorStepper_Calculator helper;
    helper._motor_gear_pitch_in_mm = 12.7;
    helper._slave_pulley_teeth_count = 368;
    float slope = helper.Get_steps_per_mm();
    __actuator_alpha.Init_FomularSlope(slope);
    __actuator_alpha.LinkStepper(board->GetStepper_Alpha(), slope);

    __actuator_beta.Init_FomularSlope(slope);
    __actuator_beta.LinkStepper(board->GetStepper_Beta(), slope);

    __actuator_gamma.LinkServo(board->GetServo_onVertical(), true);
    __actuator_gamma.Init_FomularSlope(slope);
}

void TeechWarehouse_Robot::StoreToCell(int row_index, int col_index){

}

void TeechWarehouse_Robot::RetrieveFromCell(int row_index, int col_index){
    // turn on vacuum pump
    __eef.Run(EEF_CODE_ENABLE_VACUUME);
    // air pen to top position
    __eef.Run(EEF_CODE_HIGHER);
    // Move eef to row, col
    // turn on air vacuum.
    // lower air pen
    // eef run a small circle.
    // upper air_pen
    // eef run a middle circle, to remove multi-sucked-teeth.
    // air pen to top position
    // eef move to center output-box
    // turn off air_vacume_switch, to let the tooth drop into output-box
}

void TeechWarehouse_Robot::__MoveToCell(int row_index, int col_index){
    // Constrctu Gcode from (row, col) to (X,Y)
    String g1 = "G1X";
    float x = 40.0f * row_index + 25;
    float y= 40.0f * (col_index -4);
    if(col_index>=4) y += 35.0f;
    if(col_index<=3) y -= 35.0f;
    g1.concat(x);
    g1.concat("Y");
    g1.concat(y);
    g1.concat("F0.1");
    this->_gcode_queue->AppendGcodeCommand(g1);
}