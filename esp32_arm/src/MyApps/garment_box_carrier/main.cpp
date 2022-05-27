#include "all_devices.h"
#ifdef I_AM_GARMENT_BOX_CARRIER

// #include "box_carrier_board.h"

#include "MyLibs/MyFunctions.hpp"
#include "box_carrier.h"
#include "box_carrier_hw.h"
#include "IoT/main_mqtt.h"
#include "MyApps/garment_bot_single_mcu/board_all_in_one.h"


BoxCarrier *robot;
BoxCarrierHardware* robot_hw;
GcodeQueue* gcode_queue;
MessageQueue* mqtt_command_queue;
StepControl objStepControl;
BoardAllInOne board = BoardAllInOne();

void setup_robot_hardware(){
    board.Init();
    robot_hw = new BoxCarrierHardware();
    robot_hw->InitMe(&board.cnc, &objStepControl);
    robot_hw->LinkHomer(&board.cnc.homer_z, &board.cnc.homer_y);
    robot_hw->LinkStepper(&board.cnc.stepper_alpha, &board.cnc.stepper_beta);
    robot_hw->InitRobot();
}


void setup(){
    Serial.begin(115200);
    Serial.println("Hi there, I am your lovely bot,  Garment box-Carrier for Jetty .  Keep smiling :)");
    setup_robot_hardware();

    robot = new BoxCarrier();
    gcode_queue = new GcodeQueue();
    robot->LinkLocalGcodeQueue_AsProducer(gcode_queue);
    robot_hw->LinkLocalGcodeQueue_AsConsumer(gcode_queue);

    setup_mqtt_block_connect();
    mqtt_command_queue = new MessageQueue();
    append_mqtt_bridge("puma/x2212/bm", mqtt_command_queue, robot); 
    setup_mqtt_on_message_receive(); 
    Serial.println ("\n  main.cpp  setup() is done. ------------------------------------ \n");

    robot->ExecuteMqttCommand("G28Y");
}

void loop(){
    robot->SpinOnce();
    robot_hw->SpinOnce();
    loop_mqtt();
}

#endif