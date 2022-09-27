#include "solution.h"
#include "../cnc/actuator_alpha_beta.h"
#include "mover.h"

// void GobotMainCncSolution::Init(CncBoardBase* board){
//     Logger::Halt("GobotMainCncSolution::Init(CncBoardBase* board)");
// }

void GobotMainCncSolution::Init(GobotMain_Board* cnc_board, StepControl* step_control){
    this->__mover.Init(cnc_board, step_control);
    this->_mover_base = &this->__mover;
    
    // this->_LinkEef(cnc_board->GetEef());
    this->_config = &this->__config;
    // this->_config_base = &this->__config;

    cnc_board->EnableMotor(AXIS_ALPHA, false);
	cnc_board->EnableMotor(AXIS_BETA, false);
    
    HomingConfig* config = this->_cnc_homer.GetAxisHomer(AXIS_ALPHA)->GetHomingConfig(); // GetAxisHomers()->GetAxisHomer(AXIS_ALPHA)->GetHomingConfig();
    config->IsDirectionToMax = true;
    // config->LastHomedPosition
    config->Accelleration = DEG_TO_RAD * 30;
    config->DistanceToGo = 9.9f;
    config->Speed = DEG_TO_RAD * 10;


	this->_cnc_board = cnc_board;
    this->__SelfTest();
}

void GobotMainCncSolution::__SelfTest(){
    this->_config_base.PrintOut("GobotMainCncSolution:: config");
    this->_mover_base->PrintOut("GobotMainCncSolution:: mover");
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
