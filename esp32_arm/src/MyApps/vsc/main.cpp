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


void test_board(){
    Vsc_BoardTest tester;
    tester.LinkBoard(&board);
    tester.Test_AngleSensor(0);
    tester.Test_AllHomers(0);
    tester.Test_MotorDriver(0);
    tester.Test_Offset(0);
    Serial.println("[Info] test_board() is done.");
}

void test_cnc(){
    cnc.RunG28(AXIS_ALPHA);

    while (true){
        cnc.SpinOnce();
    }
}


void setup(){
    board.Init(true);
    test_board();
    cnc.Init(&board);
    test_cnc();
    robot.LinkLocalGcodeQueue_AsProducer(&gcode_queue);
    cnc.LinkLocalGcodeQueue_AsConsumer(&gcode_queue);
    gcode_queue.AppendGcodeCommand("G28A");

    setup_mqtt_block_connect();
    append_mqtt_bridge("vsc", &mqtt_command_queue, &robot); 
    setup_mqtt_on_message_receive(); 
    Serial.println ("\n[Info] VSC   setup() is done. ------------------------------------ \n");

}

void loop(){
    //TODO  motor.spinonce()   or board.spinonce()
    robot.SpinOnce();
    cnc.SpinOnce();
    loop_mqtt();

}

#endif
