#include "solution.h"
#include "../cnc/actuator_alpha_beta.h"
#include "mover.h"

void GobotMainCncSolution::Init(CncBoardBase* board){
    Logger::Halt("GobotMainCncSolution::Init(CncBoardBase* board)");
}

void GobotMainCncSolution::Init(GobotMain_Board* board, StepControl* step_control){
    this->__mover.Init(board, step_control);
    this->_mover = &this->__mover;
    
    this->_LinkEef(board->GetEef());
    this->_config = &this->__config;
    this->_config_base = &this->__config;
    this->_Init_FiverBarBase(board);

    this->__SelfTest();
}

void GobotMainCncSolution::__SelfTest(){
    this->_config_base->PrintOut("GobotMainCncSolution:: config");
    this->_mover->PrintOut("GobotMainCncSolution:: mover");
}

EnumAxis GobotMainCncSolution::ConvertToEnum(char axis){
    switch (axis){
        case 'A':
            return AXIS_ALPHA;
            break;
        case 'B':
            return AXIS_BETA;
            break;
        case 'X':
            return AXIS_X;
            break;
        case 'Y':
            return AXIS_Y;
            break;
        default:
            Serial.print("[Error] GobotMainCncSolution::ConvertToEnum() Unknown axis= ");
            Serial.println(axis);
            break;
    }
    return AXIS_ALPHA;
}
