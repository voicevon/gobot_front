#include "all_devices.h"
#ifdef I_AM_GARMENT_BOT


#include "MyLibs/MyFunctions.hpp"
#include "garment_bot.h"
#include "IoT/main_mqtt.h"


GarmentBot *garment_robot;
MessageQueue* garment_bot_message_queue;


void setup(){
    Serial.begin(115200);
    Serial.println("Hi there, I am your lovely bot,  Garmentbot, include AGV.  Keep smiling :)");
    garment_robot = new GarmentBot();
    garment_robot->Init();

    garment_bot_message_queue =  new MessageQueue();
    setup_mqtt_block_connect();
    append_mqtt_link("garment/2212/bot", garment_bot_message_queue, garment_robot);
    Serial.println ("\n  main.cpp  setup() is done. ------------------------------------ \n");
}


void loop(){
    garment_robot->SpinOnce();
    loop_mqtt();
}

#endif