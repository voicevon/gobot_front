#include "all_applications.h"
#ifdef I_AM_TEETH_WAREHOUSE
#include "MyApps/teeth_wh/board/teeth_wh_board.h"
// #include "MyApps/vsc/board/board_tester.h"
#include "MyLibs/MyFunctions.hpp"
#include "IoT/main_mqtt.h"
#include "MyApps/teeth_wh/teeth_wh_app.h"
#include "MyApps/teeth_wh/robot/teeth_wh_robot.h"


TeethWarehouse_Board board;
GcodeQueue gcode_queue;
MessageQueue mqtt_command_queue;

TeethWarehouse_App _app;
TeechWarehouse_Robot _robot;

// Encoder encoder = Encoder(PIN_ENCODER_A, PIN_ENCODER_B, 360);
// void doA(){encoder.handleA();}
// void doB(){encoder.handleB();}


void test_board(){
    // Vsc_BoardTest tester;
    board.Test_PositionTriggers(0);
    // board.Test_HBridge(0);
    // tester.LinkBoard(&board);
    // tester.Test_EncoderSensor(board.GetAngleSensor()->GetRawSensor(), 0);
    // tester.Test_AngleSensor();
    // tester.Test_EncoderSensor(&encoder, 0);
    // tester.Test_SinglePositionTrigger(board.GetSingleHomer(AXIS_ALPHA), 0);
    // tester.Test_AxisHomer(board.GetCncHomers()->GetAxisHomer(AXIS_ALPHA), 0);

    // tester.Test_MotorDriver(board.GetActuator(AXIS_ALPHA), 0);
    // tester.Test_Offset(0);
    Serial.println("[Info] test_board() is done.");
}

// void setup_encoder(){
    // encoder.quadrature = Quadrature::ON;
//     encoder.pullup = Pullup::USE_INTERN;
//     encoder.init();
//     encoder.enableInterrupts(doA, doB);
// }

void setup(){
    
    // setup_encoder();
    // board.LinkEncoderSensor(&encoder);
    board.Init(true);

    // test_board();
    _robot.Init(&board);
    _robot.LinkLocalGcodeQueue_AsConsumer(&gcode_queue);
    _app.LinkLocalGcodeQueue_AsProducer(&gcode_queue);

    setup_mqtt_block_connect();
    append_mqtt_bridge("teeth_wh/j4", &mqtt_command_queue, &_app); 
    setup_mqtt_on_message_receive(); 
    // gcode_queue.AppendGcodeCommand("G28A");
    Logger::Info ("VSC-XiaoJuan   setup() is done. ");
}

void loop(){
    // Logger::Print("Arduino loop() point ", 1);
    _app.SpinOnce();
    // Logger::Print("Aruino loop() point ", 2);
    _robot.SpinOnce();
    // Logger::Print("Arduino loop() point ", 3);
    loop_mqtt();
    // Logger::Print("Arduino loop() point ", 4);

}

#endif
