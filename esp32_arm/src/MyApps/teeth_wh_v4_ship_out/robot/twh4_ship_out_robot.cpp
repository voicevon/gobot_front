#include "twh4_ship_out_robot.h"
#include "Robot/mcode_runner/mcode_os.h"
#include "CNC/Actuator/dc_motor/actuator_encoder_calculator.h"
#include "IoT/wifi_mqtt_client.h"


void Twh4_ShipOut_Robot::MySpinOnce(){

}

void Twh4_ShipOut_Robot::Init(Twh4_ShipOut_Board* board){
    Logger::Debug("Twh4_ShipOut_Robot::Init()");
        Logger::Debug("Twh2_Robot::Init()");
    this->_InitStatic_Queues();
    // this->__InitStatic_Actuators(board);
    this->_Init_ArmSolution();

    this->_LinkMover(&__mover);
    
    MoveBlock __all_move_blocks[QUEUE_PLANNER_BLOCK_COUNT]; 
    LineSegment __all_line_segments[QUEUE_LINE_SEGMENT_COUNT];

    // this->_g28_runner = &this->__g28_runner;
    // __g28_runner.Init(&__mover, &__arm_solution);

    // this->__m42_runner.Init(board);
    // McodeOS::Instance().LinkRunner(&this->__m42_runner);
    McodeOS::Instance().LinkJsonLoader_ForM408Runner(&this->__json_loader);
    __board = board;
}
