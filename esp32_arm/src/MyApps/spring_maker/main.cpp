#include "all_applications.h"
#ifdef I_AM_SPRING_MAKER
#include "board/board.h"
#include "MyLibs/MyFunctions.hpp"
#include "IoT/main_mqtt.h"
#include "spring_maker_app.h"
#include "robot/spring_maker_robot.h"

Board_SpringMaker board;
// SpringMaker_CncSoution cnc;
SpringMakerApp app;
SpringMakerRobot robot;
GcodeQueue gcode_queue;
MessageQueue mqtt_command_queue;

long high_count =1;
long low_count =1;

void setup(){
    board.Init(true);
    // cncMachine.Init(AXIS_ALPHA);
    // cnc.Init(&board);
    app.LinkLocalGcodeQueue_AsProducer(&gcode_queue);
    robot.LinkLocalGcodeQueue_AsConsumer(&gcode_queue);

    setup_mqtt_block_connect();
    append_mqtt_bridge("spring/maker", &mqtt_command_queue, &app); 
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
    app.SpinOnce();
    loop_mqtt();
}

#endif