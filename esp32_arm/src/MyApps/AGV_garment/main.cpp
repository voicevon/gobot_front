#include "all_devices.h"
#ifdef I_AM_GARMENT_BOT


#include "MyLibs/MyFunctions.hpp"
#include "garment_bot.h"
#include "main_mqtt.h"


GarmentBot *mybot; // = GarmentBot();
GcodeQueue* gcode_queue;
MessageQueue* message_queue;


void setup(){
    Serial.begin(115200);
    Serial.println("Hi there, I am your lovely bot,  Garmentbot AGV + BoxMover.  Keep smiling :)");
    gcode_queue = new GcodeQueue();
    message_queue = new MessageQueue();
    mybot = new GarmentBot();
    mybot->Init();
    // mybot->objBoxMover.LinkLocalMessageQueue(gcode_queue);
    // mybot->objTwinWheelHardware.LinkLocalMessageQueue(message_queue);
    // setup_mqtt(gcode_queue, message_queue);

    Serial.println ("\n  main.cpp  setup() is done. ------------------------------------ \n");
}


void loop(){
    mybot->SpinOnce();
    // loop_mqtt();
}

#endif