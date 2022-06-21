#include "all_applications.h"
#ifdef I_AM_VSC
#include "Myboards/vsc/board_vsc.h"

#include "MyLibs/MyFunctions.hpp"
#include "IoT/main_mqtt.h"
#include "vsc_robot.h"

Vsc_board board;
VscRobot robot;
MessageQueue mqtt_command_queue;

void setup(){
    board.Init(true);
    // robot.LinkLocalGcodeQueue_AsProducer(&gcode_queue);
    // cnc.LinkLocalGcodeQueue_AsConsumer(&gcode_queue);

    setup_mqtt_block_connect();
    append_mqtt_bridge("spring/maker", &mqtt_command_queue, &robot); 
    setup_mqtt_on_message_receive(); 
    Serial.println ("\n[Info] Spring Maker.setup() is done. ------------------------------------ \n");

}

void loop(){
    robot.SpinOnce();
    // cnc.SpinOnce();
    loop_mqtt();

}

#endif
