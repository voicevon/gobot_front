#include "all_applications.h"
#ifdef I_AM_VSC
#include "MyApps/vsc/board/board_vsc.h"
// #include "MyApps/vsc/board/board_tester.h"
#include "MyLibs/MyFunctions.hpp"
#include "IoT/from_mqtt_client_to_remote_queue.h"
// #include "IoT/main_mqtt.h"
#include "MyApps/vsc/vsc_app.h"
#include "MyApps/vsc/robot/vsc_robot.h"


Vsc_Board board;
GcodeQueue gcode_queue;
MessageQueue mqtt_command_queue;

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
    board.Init(true);

    test_board();
    robot.Init(&board);
    robot.LinkLocalGcodeQueue_AsConsumer(&gcode_queue);
    app.LinkLocalGcodeQueue_AsProducer(&gcode_queue);
    app.LinkRobot(&robot);

    // setup_mqtt_block_connect();
    setup_mono_remote_queue_via_mqtt("vsc/j4", &mqtt_command_queue, &app); 
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
    mono_remote_queue_spin_once();
    // Logger::Print("Arduino loop() point ", 4);

}

#endif
