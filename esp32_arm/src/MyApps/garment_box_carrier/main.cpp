#include "all_devices.h"
#ifdef I_AM_GARMENT_BOX_CARRIER


#include "MyLibs/MyFunctions.hpp"
#include "box_carrier.h"
#include "box_carrier_hw.h"
#include "IoT/main_mqtt.h"

BoxCarrier *robot;
BoxCarrierHardware* robot_hw;
GcodeQueue* gcode_queue;
MessageQueue* mqtt_command_queue;

void setup(){
    Serial.begin(115200);
    Serial.println("Hi there, I am your lovely bot,  Garment-BoxMover.  Keep smiling :)");
    robot_hw = new BoxCarrierHardware();
    robot = new BoxCarrier();
    gcode_queue = new GcodeQueue();
    robot->LinkLocalGcodeQueue_AsProducer(gcode_queue);
    robot_hw->LinkLocalGcodeQueue_AsConsumer(gcode_queue);

    setup_mqtt_block_connect();
    mqtt_command_queue = new MessageQueue();
    append_mqtt_bridge("puma/x2212/bm", mqtt_command_queue, robot); 
    setup_mqtt_on_message_receive(); 
    Serial.println ("\n  main.cpp  setup() is done. ------------------------------------ \n");
}

void loop(){
    robot->SpinOnce();
    robot_hw->SpinOnce();
    loop_mqtt();
}

#endif