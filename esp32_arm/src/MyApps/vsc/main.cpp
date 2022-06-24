#include "all_applications.h"
#ifdef I_AM_VSC
#include "board/board_vsc.h"
#include "board/board_tester.h"
#include "board/cnc_machine.h"
#include "CNC/solution/cnc_single_axis/cnc_single_axis.h"
#include "MyLibs/MyFunctions.hpp"
#include "IoT/main_mqtt.h"
#include "vsc_robot.h"

VscMachine cncMachine;
CncSingleAxis cnc = CncSingleAxis();

Vsc_Board board;
VscRobot robot;
GcodeQueue gcode_queue;
MessageQueue mqtt_command_queue;


void test_board(){
    Vsc_BoardTest tester;
    tester.LinkBoard(&board);
    tester.Test_AngleSensor(0);
    tester.Test_AllHomers(10);
    tester.Test_Motor(0);
    tester.Test_Offset(0);
    Serial.println("[Info] test_board() is done.");
}

void home(){
    SingleAxisHomer* homer = board.GetHomer('A');
    ActuatorDcMotor* motor = (ActuatorDcMotor*)(board.GetActuator('A'));
    motor->SetSpeed(100);
    motor->SetTargetPositionTo(false, 999999);
    motor->StartToMove();
    while (homer->IsTriged()){
        motor->Stop();
        motor->SetCurrentPositionAs(15);
    }
}

void setup(){
    board.Init(true);
    test_board();
    cnc.Init(&board);

    robot.LinkLocalGcodeQueue_AsProducer(&gcode_queue);
    cnc.LinkLocalGcodeQueue_AsConsumer(&gcode_queue);
    home();
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
