
#include "board/board_vsc.h"
#include "MyLibs/MyFunctions.hpp"

#include "vsc_app.h"
#include "robot/vsc_robot.h"

#include "all_applications.h"
#ifdef I_AM_VSC
Vsc_Board board;
GcodeQueue gcode_queue;
// MessageQueue mqtt_command_queue;

VscApp app;
VscRobot robot;

Encoder encoder = Encoder(PIN_ENCODER_A, PIN_ENCODER_B, 360);
void doA(){encoder.handleA();}
void doB(){encoder.handleB();}


void test_board(){
    // Vsc_BoardTest tester;
    board.Test_PositionTriggers(0);
    board.Test_HBridge(0);
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

void setup_encoder(){
    encoder.quadrature = Quadrature::ON;
    encoder.pullup = Pullup::USE_INTERN;
    encoder.init();
    encoder.enableInterrupts(doA, doB);
}

void setup(){
    
    setup_encoder();
    board.LinkEncoderSensor(&encoder);
    board.Init();

    test_board();
    robot.Init(&board);
    robot.LinkLocalGcodeQueue_AsConsumer(&gcode_queue);
    // app.LinkLocalGcodeQueue_AsProducer(&gcode_queue);
    app.LinkRobot(&robot);

    // setup_mqtt_block_connect();
    // mono_remote_queue_bridge_via_mqtt_setup("vsc/j4", &mqtt_command_queue, &app); 
    // setup_mqtt_on_message_receive(); 
    // gcode_queue.AppendGcodeCommand("G28A");
    Logger::Info ("VSC-XiaoJuan   setup() is done. ");
}

void loop(){
    // Logger::Print("Arduino loop() point ", 1);
    app.SpinOnce();
    // Logger::Print("Aruino loop() point ", 2);
    robot.SpinOnce();
    // Logger::Print("Arduino loop() point ", 3);
    // mono_remote_queue_bridge_spin_once();
    // Logger::Print("Arduino loop() point ", 4);

}

#endif
