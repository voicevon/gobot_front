#include "all_applications.h"
#ifdef I_AM_GARMENT_BOX_MOVER

#include "cnc/solution_config.h"
#include "MyApps/asar_agv/board/sub_board_cnc2205/board_cnc_2205.h"
#include "MyLibs/MyFunctions.hpp"
#include "box_mover.h"
#include "cnc/solution.h"
#include "IoT/main_mqtt.h"

// BoxMoverCncMachine machine;
Board2205Cnc board = Board2205Cnc();
BoxMover_CncSoution cnc;
BoxMover robot = BoxMover();
GcodeQueue gcode_queue;
MessageQueue mqtt_command_queue;

void setup(){
    board.Init(true);
    // cnc.Init(&board);

    cnc.LinkLocalGcodeQueue_AsConsumer(&gcode_queue);
    robot.LinkLocalGcodeQueue_AsProducer(&gcode_queue);

    setup_mqtt_block_connect();
    append_mqtt_bridge("puma/x2212/bm", &mqtt_command_queue, &robot); 
    setup_mqtt_on_message_receive(); 
    Serial.println ("\n{Info] main.cpp  setup() is done. ------------------------------------ \n");
}

void loop(){
    robot.SpinOnce();
    cnc.SpinOnce();
    loop_mqtt();
}

#endif