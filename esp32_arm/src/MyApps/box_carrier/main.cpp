#include "all_applications.h"
#ifdef I_AM_GARMENT_BOX_CARRIER


// #include "MyBoards/asrs_agv_2205/board_asrs_agv.h"
#include "MyBoards/asrs_agv_2205/board_all_in_one_2205.h"
#include "cnc_machine.h"
#include "CNC/cnc_core_yz/cnc_core_yz.h"
#include "MyLibs/MyFunctions.hpp"
#include "box_carrier.h"
#include "IoT/main_mqtt.h"


BoardAllInOne board = BoardAllInOne();
BoxCarrierMachine cncMachine;
Cnc_CoreYZ cnc = Cnc_CoreYZ();
BoxCarrier robot = BoxCarrier();
GcodeQueue gcode_queue = GcodeQueue();
MessageQueue mqtt_command_queue = MessageQueue();
StepControl objStepControl;


void setup(){
    board.Init(true);
    cncMachine.Init('A');
    cnc.Init(&board.cnc_board, &cncMachine);
    cnc.LinkStepControl(&objStepControl);
    robot.LinkLocalGcodeQueue_AsProducer(&gcode_queue);
    cnc.LinkLocalGcodeQueue_AsConsumer(&gcode_queue);

    setup_mqtt_block_connect();
    append_mqtt_bridge("puma/x2212/bm", &mqtt_command_queue, &robot); 
    setup_mqtt_on_message_receive(); 
    Serial.println ("\n[Info] box carrier  setup() is done. ------------------------------------ \n");

    robot.ExecuteMqttCommand("G28Y");
}

void loop(){
    robot.SpinOnce();
    cnc.SpinOnce();
    loop_mqtt();
}

#endif