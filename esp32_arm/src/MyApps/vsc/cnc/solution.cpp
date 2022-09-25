#include "solution.h"

Vsc_CncSoution::Vsc_CncSoution(){

}

void Vsc_CncSoution::Init(Vsc_Board* board ){
    this->_AXIS = AXIS_ALPHA;

    this->__mover.Init(board);
    this->_mover_base = &this->__mover;
    
    this->_LinkEef(board->GetEef());


    // PIDs remote configable, Link to end user, and initial setting
    this->__all_pids.AppendPidController(&this->__speed_pid);
    //So all pid controllers are configable via mcode. example: 'M130 N0 P1 I2 D3'
    this->_LinkPidControllers(&this->__all_pids);
    // motor is the user of PID controller
    board->LinkSpeedPid_ForMotor(&this->__speed_pid);

    this->__speed_pid.P = 10;
    this->__speed_pid.I = 1;
    this->__speed_pid.D = 0;


    board->EnableMotor(AXIS_ALPHA, false);

    Logger::Debug("Vsc_CncSoution::Init() Kinematic_config");
    CircleLoop_KinematicConfig* kinematic = &this->_kinematic_config;
    kinematic->CircleLength = 12.7 * 368;
    kinematic->PitchLength = 12.7;  

    Logger::Debug("Vsc_CncSoution::Init() Hoiming_config");
    HomingConfig* homing = this->_cnc_homer.GetAxisHomer(AXIS_ALPHA)->GetHomingConfig();
    homing->IsDirectionToMax = true;
    homing->Speed = TWO_PI / 60;  // 60 seconds Per loop.
    homing->Accelleration = 0.1;
    homing->DistanceToGo = TWO_PI;
    
    Logger::Debug("Vsc_CncSoution::Init() Alpha_home_triggers");
    AxisHomer* alpha = this->_cnc_homer.GetAxisHomer(AXIS_ALPHA);
    alpha->AppendPositionTrigger(board->GetPositionTrigger(0));
    alpha->AppendPositionTrigger(board->GetPositionTrigger(1));

	this->_cnc_board = board;

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
