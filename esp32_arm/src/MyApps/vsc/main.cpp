#include "all_applications.h"
#ifdef I_AM_VSC
#include "MyApps/vsc/board/board_vsc.h"
#include "MyApps/vsc/board/board_tester.h"
#include "MyApps/vsc/cnc/vsc_arm_solution.h"
#include "MyLibs/MyFunctions.hpp"
#include "IoT/main_mqtt.h"
#include "MyApps/vsc/vsc_app.h"
#include "MyApps/vsc/robot/vsc_robot.h"

Vsc_Board board;
VscApp vsc_app;

VscRobot vsc_robot;

GcodeQueue gcode_queue;
MessageQueue mqtt_command_queue;


Encoder encoder = Encoder(PIN_ENCODER_A, PIN_ENCODER_B, 360);
void doA(){encoder.handleA();}
void doB(){encoder.handleB();}


void test_board(){
    Vsc_BoardTest tester;
    board.Test_PositionTriggers(0);
    // tester.LinkBoard(&board);
    tester.Test_EncoderSensor(board.GetAngleSensor()->GetRawSensor(), 0);
    // tester.Test_AngleSensor();
    // tester.Test_EncoderSensor(&encoder, 0);
    // tester.Test_SinglePositionTrigger(board.GetSingleHomer(AXIS_ALPHA), 0);
    // tester.Test_AxisHomer(board.GetCncHomers()->GetAxisHomer(AXIS_ALPHA), 0);

    tester.Test_MotorDriver(board.GetActuator(AXIS_ALPHA), 0);
    tester.Test_Offset(0);
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
    vsc_robot.Init(&board);
    vsc_robot.LinkLocalGcodeQueue_AsConsumer(&gcode_queue);
    vsc_app.LinkLocalGcodeQueue_AsProducer(&gcode_queue);

    setup_mqtt_block_connect();
    append_mqtt_bridge("vsc/j4", &mqtt_command_queue, &vsc_app); 
    setup_mqtt_on_message_receive(); 
    // gcode_queue.AppendGcodeCommand("G28A");
    Logger::Info ("VSC-XiaoJuan   setup() is done. ");
}

void loop(){
    ActuatorDcMotor* motor = board.GetActuator(AXIS_ALPHA);
    motor->SpinOnce();
    vsc_app.SpinOnce();
    vsc_robot.SpinOnce();
    loop_mqtt();

}

#endif
