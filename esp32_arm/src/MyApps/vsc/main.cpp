#include "all_applications.h"
#ifdef I_AM_VSC
#include "board/board_vsc.h"
#include "board/board_tester.h"
#include "cnc/solution.h"
#include "MyLibs/MyFunctions.hpp"
#include "IoT/main_mqtt.h"
#include "vsc_robot.h"

Vsc_CncSoution cnc;

Vsc_Board board;
VscRobot robot;
GcodeQueue gcode_queue;
MessageQueue mqtt_command_queue;


Encoder encoder = Encoder(PIN_ENCODER_A, PIN_ENCODER_B, 360);
void doA(){encoder.handleA();}
void doB(){encoder.handleB();}


void test_board(){
    Vsc_BoardTest tester;
    // tester.LinkBoard(&board);
    tester.Test_EncoderSensor(board.GetAngleSensor(), 0);
    // tester.Test_EncoderSensor(&encoder, 0);
    tester.Test_AllHomers(board.GetHomer(AXIS_ALPHA), 0);
    tester.Test_MotorDriver(board.GetActuator(AXIS_ALPHA), 0);
    tester.Test_Offset(0);
    Serial.println("[Info] test_board() is done.");
}

void setup_encoder(){
    encoder.quadrature = Quadrature::ON;
    encoder.pullup = Pullup::USE_INTERN;
    // encoder.pullup = Pullup::USE_EXTERN;
    encoder.init();
    encoder.enableInterrupts(doA, doB);
}


void setup(){
    board.Init(true);
    setup_encoder();
    board.LinkEncoderAsSensor(&encoder);
    test_board();
    cnc.Init(&board);
    robot.LinkLocalGcodeQueue_AsProducer(&gcode_queue);
    cnc.LinkLocalGcodeQueue_AsConsumer(&gcode_queue);

    setup_mqtt_block_connect();
    append_mqtt_bridge("JXYStock", &mqtt_command_queue, &robot); 
    setup_mqtt_on_message_receive(); 
    Logger::Info ("VSC-XiaoJuan   setup() is done. ");

    //test robot and cnc
    gcode_queue.AppendGcodeCommand("G28A");

}

void loop(){
    board.GetActuator(AXIS_ALPHA)->SpinOnce();
    robot.SpinOnce();
    cnc.SpinOnce();
    loop_mqtt();

}

#endif
