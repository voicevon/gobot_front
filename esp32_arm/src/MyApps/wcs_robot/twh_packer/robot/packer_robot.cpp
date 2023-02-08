#include "packer_robot.h"
#include "Robot/mcode_runner/mcode_os.h"
#include "CNC/Actuator/dc_motor/actuator_encoder_calculator.h"
#include "Mqtt/wifi_mqtt_client.h"


void Twh_Packer_Robot::MySpinOnce(){

}

void Twh_Packer_Robot::Init(Twh_Packer_Board* board){
    Logger::Debug("Twh_Packer_Robot::Init()");
    this->_InitStatic_Queues();
    this->_Init_ArmSolution();
    this->_LinkMover(&__mover);
    


    this->_g28_runner = &this->__g28_runner;
    __g28_runner.Init(&__mover, &__arm_solution);

    this->__m42_runner.Init(board);
    McodeOS::Instance().LinkRunner(&__m42_runner);
    McodeOS::Instance().LinkJsonLoader_ForM408Runner(&__json_loader);
    __board = board;
}

void Twh_Packer_Robot::_InitStatic_Queues(){
    Queue_MoveBlock::Instance()._all_queue_ables = (Queue_able*)this->__all_move_blocks;
    // Init LineSegment queue head
    Queue_LineSegment::Instance()._all_queue_ables = (Queue_able*) this->__all_line_segments;
    LineSegment* line = Queue_LineSegment::Instance().GetRoom();
    Queue_LineSegment::Instance().Deposit();
    Logger::Print("Twh_Packer_Robot::Init", 83);
}


void Twh_Packer_Robot::_Init_ArmSolution(){
    this->_LinkArmSolution_for_planner(&__arm_solution);
}