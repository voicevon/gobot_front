#include "all_applications.h"
#ifdef I_AM_SPRING_MAKER
#include "board/board.h"
#include "MyLibs/MyFunctions.hpp"
#include "IoT/main_mqtt.h"
#include "spring_maker_app.h"
#include "robot/spring_maker_robot.h"

Board_SpringMaker board;
SpringMakerApp app;
SpringMakerRobot robot;
GcodeQueue gcode_queue;
MessageQueue mqtt_command_queue;

long high_count =1;
long low_count =1;

void setup(){
    board.Init(true);
    robot.Init(&board);    
    app.LinkLocalGcodeQueue_AsProducer(&gcode_queue);
    robot.LinkLocalGcodeQueue_AsConsumer(&gcode_queue);

    // connect_to_mqtt_broker();
    setup_mono_remote_queue_via_mqtt("spring/maker", &mqtt_command_queue, &app); 
    // setup_mqtt_on_message_receive(); 
    Logger::Info ("\nSpring Maker.setup() is done. ------------------------------------ \n");
}

void loop(){
    robot.SpinOnce();
    app.SpinOnce();
    loop_mqtt();
}

#endif