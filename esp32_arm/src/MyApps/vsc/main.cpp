#include "all_applications.h"
#ifdef I_AM_VSC
#include "board/board_vsc.h"
#include "board/board_tester.h"
#include "MyLibs/MyFunctions.hpp"
#include "IoT/main_mqtt.h"
#include "vsc_robot.h"

Vsc_Board board;
VscRobot robot;
MessageQueue mqtt_command_queue;


void test_board(){
    Vsc_BoardTest tester;
    tester.LinkBoard(&board);
    tester.Test_AngleSensor(100);

    tester.Test_AllHomers(81);
    // tester.Test_Motor(20);

}
void setup(){
    board.Init(true);
    test_board();
    // robot.LinkLocalGcodeQueue_AsProducer(&gcode_queue);
    // cnc.LinkLocalGcodeQueue_AsConsumer(&gcode_queue);

    setup_mqtt_block_connect();
    append_mqtt_bridge("spring/maker", &mqtt_command_queue, &robot); 
    setup_mqtt_on_message_receive(); 
    Serial.println ("\n[Info] Spring Maker.setup() is done. ------------------------------------ \n");

}

void loop(){
    robot.SpinOnce();
    // cnc.SpinOnce();
    loop_mqtt();

}

#endif
