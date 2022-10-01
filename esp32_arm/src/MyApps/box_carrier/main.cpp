#include "all_applications.h"
#ifdef I_AM_GARMENT_BOX_CARRIER


#include "MyApps/asar_agv/board/board_all_in_one_2205.h"
#include "MyLibs/MyFunctions.hpp"
#include "box_carrier_app.h"
#include "IoT/main_mqtt.h"
#include "robot/box_carrier_robot.h"


BoardAllInOne board = BoardAllInOne();
BoxCarrierApp app;
GcodeQueue gcode_queue = GcodeQueue();
MessageQueue mqtt_command_queue = MessageQueue();
BoxCarrierRobot robot;


void setup(){
    board.Init(true);
    // cncMachine.Init('A');
    // cnc.Init(&board.cnc_board);
    // cnc.LinkStepControl(&objStepControl);
    app.LinkLocalGcodeQueue_AsProducer(&gcode_queue);
    robot.LinkLocalGcodeQueue_AsConsumer(&gcode_queue);

    setup_mqtt_block_connect();
    append_mqtt_bridge("puma/x2212/bm", &mqtt_command_queue, &app); 
    setup_mqtt_on_message_receive(); 
    Serial.println ("\n[Info] box carrier  setup() is done. ------------------------------------ \n");

    app.AsyncExecuteMqttCommand("G28Y");
}

void loop(){
    robot.SpinOnce();
    app.SpinOnce();
    loop_mqtt();
}

#endif