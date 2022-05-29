#include "all_devices.h"
#ifdef I_AM_GARMENT_BOX_MOVER

#include "MyBoards/asrs_agv_2205/board_2205_cnc.h"
#include "MyLibs/MyFunctions.hpp"
#include "box_mover.h"
#include "CNC/cnc_core_az/box_mover_hw.h"
#include "IoT/main_mqtt.h"

Board2205Cnc board = Board2205Cnc();
CncCoreAZ robot_hw = CncCoreAZ();
BoxMover robot = BoxMover();
GcodeQueue gcode_queue;
MessageQueue mqtt_command_queue;

void setup(){
    Serial.begin(115200);
    Serial.println("Hi there, I am your lovely bot,  Garment-BoxMover.  Keep smiling :)");
    board.Init();
    robot_hw.Init(&board);
    robot.LinkLocalGcodeQueue_AsProducer(&gcode_queue);
    robot_hw.LinkLocalGcodeQueue_AsConsumer(&gcode_queue);

    setup_mqtt_block_connect();
    append_mqtt_bridge("puma/x2212/bm", &mqtt_command_queue, &robot); 
    setup_mqtt_on_message_receive(); 
    Serial.println ("\n{Info] main.cpp  setup() is done. ------------------------------------ \n");
}

void loop(){
    robot.SpinOnce();
    robot_hw.SpinOnce();
    loop_mqtt();
}

#endif