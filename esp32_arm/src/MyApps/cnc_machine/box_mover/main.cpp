

#include "MyApps/asar_agv/board/sub_board_cnc2205/board_cnc_2205.h"
#include "MyLibs/MyFunctions.hpp"
#include "box_mover_app.h"
#include "IoT/from_mqtt_client_to_remote_queue.h"
#include "robot/box_mover_robot.h"

#include "all_applications.h"
#ifdef I_AM_GARMENT_BOX_MOVER

Board2205Cnc board = Board2205Cnc();
BoxMoverApp app = BoxMoverApp();
BoxMoverRobot robot;
GcodeQueue gcode_queue;
MessageQueue mqtt_command_queue;


void setup(){
    board.Init();

    robot.LinkLocalGcodeQueue_AsConsumer(&gcode_queue);
    app.LinkLocalGcodeQueue_AsProducer(&gcode_queue);

    // connect_to_mqtt_broker();
    mono_remote_queue_bridge_via_mqtt_setup("puma/x2212/bm", &mqtt_command_queue, &app); 
    // setup_mqtt_on_message_receive(); 
    Serial.println ("\n{Info] main.cpp  setup() is done. ------------------------------------ \n");
}

void loop(){
    robot.SpinOnce();
    app.SpinOnce();
    mono_remote_queue_bridge_spin_once();
}

#endif