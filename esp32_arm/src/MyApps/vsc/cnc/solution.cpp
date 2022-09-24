#include "solution.h"

void Vsc_CncSoution::Init(Vsc_Board* board ){
    this->_AXIS = AXIS_ALPHA;

    this->__mover.Init(board);
    this->_mover_base = &this->__mover;
    
    this->_LinkEef(board->GetEef());
    Logger::Debug("aaaaaaaaaaaa");
    // this->_config->LinkHomers(board->GetCncHomers());
    // this->_linkHomer(board->GetCncHomers());
    board->GetPositionTrigger(0)->SetTriggerPosition(0);
    board->GetPositionTrigger(1)->SetTriggerPosition(TWO_PI * 90 / 368);
    board->GetPositionTrigger(2)->SetTriggerPosition(TWO_PI * 180 / 368);
    board->GetPositionTrigger(3)->SetTriggerPosition(TWO_PI * 270 / 368);

    this->_homer.GetAxisHomer(AXIS_ALPHA)->AppendPositionTrigger(board->GetPositionTrigger(0));
    this->_homer.GetAxisHomer(AXIS_ALPHA)->AppendPositionTrigger(board->GetPositionTrigger(1));
    // this->_homer.GetAxisHomer(AXIS_ALPHA)->AppendPositionTrigger(board->GetPositionTrigger(2));
    // this->_homer.GetAxisHomer(AXIS_ALPHA)->AppendPositionTrigger(board->GetPositionTrigger(3));
    Logger::Debug("bbbbbbbbb");

    //So all pid controllers are configable via mcode. example: 'M130 N0 P1 I2 D3'
    this->_LinkPidControllers(board->GetPidControllers());

    board->EnableMotor(AXIS_ALPHA, false);
	this->_cnc_board = board;

    HomingConfig* config = this->_homer.GetAxisHomer(AXIS_ALPHA)->GetHomingConfig();
    config->IsDirectionToMax = true;
    config->Speed = TWO_PI / 60;  // 60 seconds Per loop.
    config->Accelleration = 0.1;
    config->DistanceToGo = TWO_PI;

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
