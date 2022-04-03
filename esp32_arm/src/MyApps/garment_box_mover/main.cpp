#include "all_devices.h"
#ifdef I_AM_GARMENT_BOX_MOVER


#include "MyLibs/MyFunctions.hpp"
#include "box_mover.h"
#include "box_mover_hw.h"
#include "IoT/main_mqtt.h"

// Local gcode Producer: box_mover
BoxMover *robot;
// Local gcode Consumer: box_mover_hardware
BoxMoverHardware* robot_hw;
GcodeQueue* gcode_queue;



// Remote message to local
MessageQueue* mqtt_command_queue;

void setup(){
    Serial.begin(115200);
    Serial.println("Hi there, I am your lovely bot,  Garment-BoxMover.  Keep smiling :)");
    gcode_queue = new GcodeQueue();
    robot = new BoxMover();
    robot_hw = new BoxMoverHardware();
    robot->LinkLocalGcodeQueue_AsProducer(gcode_queue);
    robot_hw->LinkLocalGcodeQueue_AsConsumer(gcode_queue);

    setup_mqtt_block_connect();
    append_mqtt_link("garment/2212/bm", mqtt_command_queue, robot);   // NO NEED MQTT, PURE LOCALLY!!!
    Serial.println ("\n  main.cpp  setup() is done. ------------------------------------ \n");
}


void loop(){
    robot->SpinOnce();
    robot_hw->SpinOnce();
    loop_mqtt();
}

#endif