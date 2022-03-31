#include "all_devices.h"
#ifdef I_AM_GARMENT_BOT


#include "MyLibs/MyFunctions.hpp"
#include "garment_bot.h"
#include "main_simplefoc.h"
#include "main_mqtt.h"



#define PIN_ENCODER_A 12
#define PIN_ENCODER_B 13

GarmentBot *mybot; // = GarmentBot();
GarmentAgv agv;

GcodeQueue* gcode_queue;
MessageQueue* message_queue;

void setup(){
    Serial.begin(115200);
    Serial.println("Hi there, I am your lovely bot,  Garmentbot AGV + BoxMover.  Keep smiling :)");
    gcode_queue = new GcodeQueue();
    message_queue = new MessageQueue();
    mybot = new GarmentBot();
    mybot->Init();
    mybot->objBoxMover.LinkLocalMessageQueue(gcode_queue);
    mybot->objTwinWheelHardware.LinkLocalMessageQueue(message_queue);
    setup_mqtt(gcode_queue, message_queue);

    setup_simplefoc();
    Serial.println ("\n\n  Setup_wifi_mqtt is done. ------------------------------------ ");
}


void loop(){
    mybot->SpinOnce();

    loop_simplefoc();
    loop_mqtt();
}

#endif