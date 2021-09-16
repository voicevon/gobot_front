#include "all_devices.h"
#ifdef I_AM_CABLEBOT_CORNER

// #include "board_cable_bot_corner.h"
#include "Robot/Axis_DC_Encoder.h"
#include "MyLibs/BleHelper.h"
#include "MyLibs/Components/Led.h"
#include "SimpleFOC/SimpleFOC.h"
#include "Robot/SingleAxisBase.hpp"

Led led_power = Led(0,1,LOW);
Led led_home_alpha = Led(1,2,LOW);
BleHelper bleHelper = BleHelper();

// Board_CableBotCorner board = Board_CableBotCorner();
DCMotor motor = DCMotor();
Encoder encoder = Encoder(1,2,3);
SingleAxisBase<DCMotor,Encoder> axis = SingleAxisBase<DCMotor,Encoder>();


// void doA(){board.encoder->handleA();}
// void doB(){board.encoder->handleB();}

// void doA(){axis.handleA();}
// void doA(){axis.handleA();}


void setup(){
    axis.LinkAcuator(motor);
    axis.LinkSensor(encoder);

    // board.Flash_AllLeds(3,500,500);
    // board.flash();
    bleHelper.InitBle();
    // board.encoder->enableInterrupts(doA,doB);
    motor.controller = MotionControlType::angle;
    motor.P_angle.P = 1;

    // axis.driver = board.driver;
    // axis.encoder = board.encoder;
    axis.Init_scaler(1.234) ;

    axis.Home();
}


void loop(){
    axis.SetTargetAbs(100);
    axis.MoveAsync();
}

#endif