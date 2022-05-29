#include "all_devices.h"
#ifdef I_AM_SPRING_MAKER
#include "MyBoards/sping_maker/board_spring_maker.h"

#include "MyLibs/MyFunctions.hpp"
#include "IoT/main_mqtt.h"
#include "spring_maker.h"
#include "CNC/cnc_single_axis/cnc_single_axis.h"

Board_SpringMaker board = Board_SpringMaker();
SpringMaker *robot;
CncSingleAxis cnc = CncSingleAxis();
GcodeQueue* gcode_queue;
MessageQueue* mqtt_command_queue;

long high_count =1;
long low_count =1;

void setup(){
    Serial.begin(115200);
    Serial.println("Hi there, I am your lovely bot,  Spring-Maker.  Keep smiling :)");
    board.Init();
    
    cnc.Init(&board);
    robot = new SpringMaker();
    gcode_queue = new GcodeQueue();
    robot->LinkLocalGcodeQueue_AsProducer(gcode_queue);
    cnc.LinkLocalGcodeQueue_AsConsumer(gcode_queue);

    setup_mqtt_block_connect();
    mqtt_command_queue = new MessageQueue();
    append_mqtt_bridge("spring/maker", mqtt_command_queue, robot); 
    setup_mqtt_on_message_receive(); 
    Serial.println ("\n[Info] main.cpp  setup() is done. ------------------------------------ \n");
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
    cnc.SpinOnce();
    loop_mqtt();
}

#endif