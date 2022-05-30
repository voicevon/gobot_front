#include "all_applications.h"
#ifdef I_AM_GARMENT_BOX_CARRIER


#include "MyLibs/MyFunctions.hpp"
#include "box_carrier.h"
#include "CNC/cnc_core_yz/cnc_core_yz.h"
#include "IoT/main_mqtt.h"
#include "MyBoards/asrs_agv_2205/board_asrs_agv.h"


BoardAllInOne board = BoardAllInOne();
Cnc_CoreYZ cnc = Cnc_CoreYZ();
BoxCarrier robot = BoxCarrier();
GcodeQueue gcode_queue = GcodeQueue();
MessageQueue mqtt_command_queue = MessageQueue();
StepControl objStepControl;

void setup_robot_hardware(){

}


void setup(){
    board.Init(true);
    // Serial.println("Hi there, I am your lovely bot,  Garment box-Carrier for Jetty .  Keep smiling :)");
    cnc.Init(&board.cnc);
    robot.LinkLocalGcodeQueue_AsProducer(&gcode_queue);
    cnc.LinkLocalGcodeQueue_AsConsumer(&gcode_queue);

    setup_mqtt_block_connect();
    append_mqtt_bridge("puma/x2212/bm", &mqtt_command_queue, &robot); 
    setup_mqtt_on_message_receive(); 
    Serial.println ("\n[Info] main.cpp  setup() is done. ------------------------------------ \n");

    robot.ExecuteMqttCommand("G28Y");
}

void loop(){
    robot.SpinOnce();
    cnc.SpinOnce();
    loop_mqtt();
}

#endif