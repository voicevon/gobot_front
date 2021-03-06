#include "all_applications.h"
#ifdef I_AM_SPRING_MAKER
#include "board/board.h"
// #include "board/mechanic/cnc_machine.h"
// #include "board/mechanic/cnc_solution_config.h"
#include"cnc/solution_config.h"
#include "cnc/solution.h"
// #include "board/mechanic/cnc_solution.h"
#include "MyLibs/MyFunctions.hpp"
#include "IoT/main_mqtt.h"
#include "spring_maker.h"

Board_SpringMaker board;
// SpringMakerMachine cncMachine;
SpringMaker_CncSoution cnc;
SpringMaker robot;
GcodeQueue gcode_queue;
MessageQueue mqtt_command_queue;

long high_count =1;
long low_count =1;

void setup(){
    board.Init(true);
    // cncMachine.Init(AXIS_ALPHA);
    cnc.Init(&board);
    robot.LinkLocalGcodeQueue_AsProducer(&gcode_queue);
    cnc.LinkLocalGcodeQueue_AsConsumer(&gcode_queue);

    setup_mqtt_block_connect();
    append_mqtt_bridge("spring/maker", &mqtt_command_queue, &robot); 
    setup_mqtt_on_message_receive(); 
    Serial.println ("\n[Info] Spring Maker.setup() is done. ------------------------------------ \n");
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
    robot.SpinOnce();
    cnc.SpinOnce();
    loop_mqtt();
}

#endif