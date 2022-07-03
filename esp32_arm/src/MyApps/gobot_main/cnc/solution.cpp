#include "solution.h"
#include "../cnc/actuator_alpha_beta.h"
#include "mover.h"

void GobotMainCnc::Init(CncBoardBase* board){
    this->_Init_FiverBarBase(board);
    this->__mover.Init(board);
    Board_GobotMain* my_board = (Board_GobotMain*) board;
    this->_Init(my_board);
}

void GobotMainCnc::_Init(Board_GobotMain* board){
    GobotMain_ActuatorAlphaBetaMechanic alpha;
}


EnumAxis GobotMainCnc::ConvertToEnum(char axis){
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
            Serial.print("[Error] GobotMainCnc::ConvertToEnum() Unknown axis= ");
            Serial.println(axis);
            break;
    }
    return AXIS_ALPHA;
}
