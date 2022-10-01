#include "all_applications.h"
#ifdef I_AM_GARMENT_BOX_MOVER

#include "MyApps/asar_agv/board/sub_board_cnc2205/board_cnc_2205.h"
#include "MyLibs/MyFunctions.hpp"
#include "box_mover_app.h"
#include "IoT/main_mqtt.h"
#include "robot/box_mover_robot.h"

Board2205Cnc board = Board2205Cnc();
BoxMoverApp app = BoxMoverApp();
BoxMoverRobot robot;
GcodeQueue gcode_queue;
MessageQueue mqtt_command_queue;

void setup(){
    board.Init(true);

    robot.LinkLocalGcodeQueue_AsConsumer(&gcode_queue);
    app.LinkLocalGcodeQueue_AsProducer(&gcode_queue);

    setup_mqtt_block_connect();
    append_mqtt_bridge("puma/x2212/bm", &mqtt_command_queue, &app); 
    setup_mqtt_on_message_receive(); 
    Serial.println ("\n{Info] main.cpp  setup() is done. ------------------------------------ \n");
}

void loop(){
    robot.SpinOnce();
    app.SpinOnce();
    loop_mqtt();
}

#endif