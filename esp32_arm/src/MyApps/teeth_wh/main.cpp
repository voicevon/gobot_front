#include "all_applications.h"
#ifdef I_AM_TEETH_WAREHOUSE

#include "MyApps/teeth_wh/board/teeth_wh_board.h"
#include "MyLibs/MyFunctions.hpp"
#include "IoT/main_mqtt.h"
#include "MyApps/teeth_wh/teeth_wh_app.h"
#include "MyApps/teeth_wh/robot/teeth_wh_robot.h"


TeethWarehouse_Board board;
GcodeQueue gcode_queue;
MessageQueue mqtt_command_queue;

TeethWarehouse_App app;
TeechWarehouse_Robot robot;

void test_board(){
    board.Test_PositionTriggers(0);
    Serial.println("[Info] test_board() is done.");
}


void setup(){
    
    board.Init(true);

    // test_board();
    robot.Init(&board);
    robot.LinkLocalGcodeQueue_AsConsumer(&gcode_queue);
    app.LinkLocalGcodeQueue_AsProducer(&gcode_queue);
    app.LinkRobot(&robot);

    setup_mqtt_block_connect();
    append_mqtt_bridge("teeth_wh/wh2210", &mqtt_command_queue, &app); 
    setup_mqtt_on_message_receive(); 
    // gcode_queue.AppendGcodeCommand("G28A");
    Logger::Info ("Teeth Warehouse   setup() is done. ");
}

void loop(){
    // Logger::Print("Arduino loop() point ", 1);
    app.SpinOnce();
    // Logger::Print("Aruino loop() point ", 2);
    robot.SpinOnce();
    // Logger::Print("Arduino loop() point ", 3);
    loop_mqtt();
    // Logger::Print("Arduino loop() point ", 4);

}

#endif
