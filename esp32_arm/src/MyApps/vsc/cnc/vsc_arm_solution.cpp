#include "vsc_arm_solution.h"


void Vsc_ArmSoution::Init(Vsc_Board* board ){
    this->_AXIS = AXIS_ALPHA;

    this->__mover.Init(board);
    this->_mover_base = &this->__mover;
    this->planner = &this->__planner;
    


    Logger::Info("Vsc_ArmSoution::Init() angle sensor and motor");
    board->GetAngleSensor()->SetupFormular(0.027173913f, 0.0f);   // slope = (10 / 56) * (56/368)
    board->EnableMotor(AXIS_ALPHA, false);

    Logger::Info("Vsc_ArmSoution::Init() Kinematic_config");
    CircleLoop_KinematicConfig* kinematic = &this->_kinematic_config;
    kinematic->CircleLength = 12.7 * 368;
    kinematic->PitchLength = 12.7;  

    Logger::Info("Vsc_ArmSoution::Init() Hoiming_config");
    HomingConfig* homing = this->_cnc_homer.GetAxisHomer(AXIS_ALPHA)->GetHomingConfig();
    homing->IsDirectionToMax = true;
    homing->Speed = TWO_PI / 60;  // 60 seconds Per loop.
    homing->Accelleration = 0.1;
    homing->DistanceToGo = TWO_PI;
    
    Logger::Info("Vsc_ArmSoution::Init() Alpha axis home_triggers");
    AxisHomer* alpha = this->_cnc_homer.GetAxisHomer(AXIS_ALPHA);
    PositionTrigger* trigger;
    trigger = board->GetPositionTrigger(0);
    trigger->SetTriggerPosition(TWO_PI* 1 / 386);      // @01 pitch, total 386 pitches,    
    alpha->AppendPositionTrigger(trigger);

    trigger = board->GetPositionTrigger(1);
    trigger->SetTriggerPosition(TWO_PI * 90 / 386);     //at pitch 90 , total 386 pitches, value = TWOPI *(90/386)
    alpha->AppendPositionTrigger(trigger);

	this->_cnc_board = board;

}

EnumAxis Vsc_ArmSoution::ConvertToEnum(char axis){
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
