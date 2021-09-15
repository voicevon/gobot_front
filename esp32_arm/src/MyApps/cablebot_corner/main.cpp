
#include "MyBoards/board_cable_bot_corner.h"
#include "RobotJoint/SingleAxis.h"


Board_CableBotCorner board = Board_CableBotCorner();
DCMotor motor = DCMotor();

void doA(){};
void doB(){};

void init(){
    board.encoder->enableInterrupts(doA,doB);
    motor.controller = MotionControlType::angle;
    motor.P_angle.P = 1;
    
}


void spin_once(){

}