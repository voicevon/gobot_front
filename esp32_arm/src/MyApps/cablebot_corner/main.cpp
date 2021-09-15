
#include "MyBoards/board_cable_bot_corner.h"
#include "RobotJoint/SingleAxis.h"
#include "MyApps/Common/BleHelper.h"

Board_CableBotCorner board = Board_CableBotCorner();
DCMotor motor = DCMotor();
SingleAxis axis = SingleAxis(0);
BleHelper bleHelper = BleHelper();


void doA(){board.encoder->handleA();}
void doB(){board.encoder->handleB();}


void init(){
    bleHelper.InitBle();
    board.encoder->enableInterrupts(doA,doB);
    motor.controller = MotionControlType::angle;
    motor.P_angle.P = 1;

    axis.driver = board.driver;
    axis.encoder = board.encoder;
    axis.Init_scaler(1.234) ;

    axis.Home();
}


void spin_once(){
    axis.SetTargetAbs(100);
    axis.MoveAsync();
}