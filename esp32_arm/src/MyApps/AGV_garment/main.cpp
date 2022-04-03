#include "all_devices.h"
#ifdef I_AM_GARMENT_BOT


#include "MyLibs/MyFunctions.hpp"
#include "garment_bot.h"
#include "main_mqtt.h"


GarmentBot *mybot;
// Producer: box_mover
// Consumer: box_mover_hardware
GcodeQueue* box_mover_gcode_queue;

MessageQueue* agv_message_queue;


void setup(){
    Serial.begin(115200);
    Serial.println("Hi there, I am your lovely bot,  Garmentbot AGV + BoxMover.  Keep smiling :)");
    box_mover_gcode_queue = new GcodeQueue();
    agv_message_queue = new MessageQueue();
    mybot = new GarmentBot();
    mybot->Init();

    setup_mqtt_block_connect();
    mybot->objBoxMover.LinkLocalMessageQueue_AsProducer(box_mover_gcode_queue);  // Local mq Consumer !!!
    mqtt_box_mover_link_gcode_queue("garment/2212/bm", box_mover_gcode_queue);  // local mq Producer.
    // mybot->objAgv.LinkLocalMessageQueue(message_queue);
    mqtt_agv_link_message_queue("garment/2212/agv",agv_message_queue);  // local mq Producer.

    Serial.println ("\n  main.cpp  setup() is done. ------------------------------------ \n");
}


void loop(){
    mybot->SpinOnce();
    loop_mqtt();
}

#endif