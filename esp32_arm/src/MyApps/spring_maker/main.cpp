#include "all_devices.h"
#ifdef I_AM_SPRING_MAKER

#include "MyLibs/MyFunctions.hpp"
#include "IoT/main_mqtt.h"
#include "spring_maker.h"
#include "spring_maker_hw.h"


SpringMaker *robot;
SpringMakerHardware* robot_hw;
GcodeQueue* gcode_queue;
MessageQueue* mqtt_command_queue;

void setup(){
    Serial.begin(115200);
    Serial.println("Hi there, I am your lovely bot,  Garment-BoxMover.  Keep smiling :)");
    robot_hw = new SpringMakerHardware();
    robot = new SpringMaker();
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