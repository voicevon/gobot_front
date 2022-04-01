#include "all_devices.h"
#ifdef I_AM_GARMENT_BOT


#include "MyLibs/MyFunctions.hpp"
#include "garment_bot.h"
#include "main_simplefoc.h"
#include "main_mqtt.h"



#define PIN_ENCODER_A 12
#define PIN_ENCODER_B 13

GarmentBot *mybot; // = GarmentBot();
GcodeQueue* gcode_queue;
MessageQueue* message_queue;

HallSensor sensor_left = HallSensor(21,22,23,15);
void doA_left(){sensor_left.handleA();}
void doB_left(){sensor_left.handleB();}
void doC_left(){sensor_left.handleC();}

HallSensor sensor_right = HallSensor(21,22,23,15);
void doA_right(){sensor_right.handleA();}
void doB_right(){sensor_right.handleB();}
void doC_right(){sensor_right.handleC();}


void setup_agv_motors(){
    SingleWheel_config* config = &mybot->objAgv.objLeftWheel.config;
    config->hall_sensor = &sensor_left;
    config->hall_do_A = &doA_left;
    config->hall_do_B = &doB_left;
    config->hall_do_C = &doC_left;
    config->driver_pin_a = 1;
    config->driver_pin_b = 2;
    config->driver_pin_c = 3;
    config->driver_pin_enable = 4;

    config = &mybot->objAgv.objLeftWheel.config;
    config->hall_sensor = &sensor_right;
    config->hall_do_A = &doA_right;
    config->hall_do_B = &doB_right;
    config->hall_do_C = &doC_right;  
    config->driver_pin_a = 1;
    config->driver_pin_b = 2;
    config->driver_pin_c = 3;
    config->driver_pin_enable = 4;  
}

void setup(){
    Serial.begin(115200);
    Serial.println("Hi there, I am your lovely bot,  Garmentbot AGV + BoxMover.  Keep smiling :)");
    gcode_queue = new GcodeQueue();
    message_queue = new MessageQueue();
    mybot = new GarmentBot();
    setup_agv_motors();
    mybot->Init();
    // mybot->objBoxMover.LinkLocalMessageQueue(gcode_queue);
    // mybot->objTwinWheelHardware.LinkLocalMessageQueue(message_queue);
    // setup_mqtt(gcode_queue, message_queue);

    // setup_simplefoc();
    Serial.println ("\n  main.cpp  setup() is done. ------------------------------------ \n");
}


void loop(){
    mybot->SpinOnce();
    // loop_simplefoc();
    // loop_mqtt();
}

#endif