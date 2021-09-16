#include "all_devices.h"
#ifdef I_AM_CABLEBOT_CORNER

#include "MyBoards/board_cable_bot_corner.h"
#include "Robot/Axis_DC_Encoder.h"
#include "MyLibs/BleHelper.h"

Board_CableBotCorner board = Board_CableBotCorner();
DCMotor motor = DCMotor();
Axis_DC_Encoder axis = Axis_DC_Encoder(0);
BleHelper bleHelper = BleHelper();


void doA(){board.encoder->handleA();}
void doB(){board.encoder->handleB();}


void setup(){
    // board.Flash_AllLeds(3,500,500);
    board.flash();
    bleHelper.InitBle();
    board.encoder->enableInterrupts(doA,doB);
    motor.controller = MotionControlType::angle;
    motor.P_angle.P = 1;

    axis.driver = board.driver;
    axis.encoder = board.encoder;
    axis.Init_scaler(1.234) ;

    axis.Home();
}


void loop(){
    axis.SetTargetAbs(100);
    axis.MoveAsync();
}

#endif