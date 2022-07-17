#include "solution.h"


void Vsc_CncSoution::Init(Vsc_Board* board ){
    this->__mover.Init(board);
    this->_mover = &this->__mover;
    
    this->_LinkEef(board->GetEef());
    this->_config_base = &this->__config;

    board->EnableMotor(AXIS_ALPHA, false);
	board->EnableMotor(AXIS_BETA, false);
	this->_board = board;
}

EnumAxis Vsc_CncSoution::ConvertToEnum(char axis){
    switch (axis){
        case 'A':
            return AXIS_ALPHA;
            break;

        default:
            Serial.print("[Error] GobotMainCnc::ConvertToEnum() Unknown axis= ");
            Serial.println(axis);
            while(true){
                Serial.print("E ");
                delay(500);
            }
            return AXIS_ALPHA;
            break;
    }
}
