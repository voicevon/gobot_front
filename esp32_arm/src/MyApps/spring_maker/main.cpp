#include "all_devices.h"
#ifdef I_AM_SPRING_MAKER
#include "spring_maker_board.h"

#include "MyLibs/MyFunctions.hpp"
#include "IoT/main_mqtt.h"
#include "spring_maker.h"
#include "spring_maker_hw.h"


SpringMaker *robot;
SpringMakerHardware* robot_hw;
GcodeQueue* gcode_queue;
MessageQueue* mqtt_command_queue;

long high_count =1;
long low_count =1;

void setup(){
    Serial.begin(115200);
    Serial.println("Hi there, I am your lovely bot,  Spring-Maker.  Keep smiling :)");
    // pinMode(15,INPUT_PULLUP);
    // return;
    robot_hw = new SpringMakerHardware();
    robot_hw->InitRobot();
    Serial.println("11111111");
    robot = new SpringMaker();
    Serial.println("2222222222");
    gcode_queue = new GcodeQueue();
    Serial.println("333333333");
    robot->LinkLocalGcodeQueue_AsProducer(gcode_queue);
    Serial.println("4444444444444");
    robot_hw->LinkLocalGcodeQueue_AsConsumer(gcode_queue);
    Serial.println("5555555555");

    setup_mqtt_block_connect();
    mqtt_command_queue = new MessageQueue();
    append_mqtt_bridge("spring/maker", mqtt_command_queue, robot); 
    setup_mqtt_on_message_receive(); 
    Serial.println ("\n  main.cpp  setup() is done. ------------------------------------ \n");
}

void loop(){
    // if (digitalRead(15) == HIGH)
    //     high_count++;
    // else{
    //     low_count++;
    //     Serial.print(high_count);
    //     Serial.print("\t");
    //     Serial.print(low_count);
    //     Serial.print("\t");
    //     Serial.print(high_count/low_count);
    //     Serial.print("\t");
    // }

    // return;
    robot->SpinOnce();
    robot_hw->SpinOnce();
    loop_mqtt();
}

#endif