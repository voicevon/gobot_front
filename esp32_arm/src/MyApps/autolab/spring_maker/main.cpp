#include "board/board.h"
#include "MyLibs/MyFunctions.hpp"

#include "spring_maker_app.h"
#include "robot/spring_maker_robot.h"

#include "../select_app.h"
#ifdef I_AM_SPRING_MAKER

Board_SpringMaker board;
SpringMakerApp app;
SpringMakerRobot robot;
GcodeQueue gcode_queue;
// MessageQueue mqtt_command_queue;

long high_count =1;
long low_count =1;

void setup(){
    board.Init();
    robot.Init(&board);    
    // app.LinkLocalGcodeQueue_AsProducer(&gcode_queue);
    // robot.LinkLocalGcodeQueue_AsConsumer(&gcode_queue);
//
    // connect_to_mqtt_broker();
    // mono_remote_queue_bridge_via_mqtt_setup("spring/maker", &mqtt_command_queue, &app); 
    // setup_mqtt_on_message_receive(); 
    Logger::Info ("\nSpring Maker.setup() is done. ------------------------------------ \n");
}

void loop(){
    robot.SpinOnce();
    app.SpinOnce();
    // mono_remote_queue_bridge_spin_once();
}

#endif