#include "all_devices.h"
#ifdef I_AM_GOBOT_HOUSE

#include "MyBoards/gobot_house/board_gobot_house.h"
#include "CNC/cnc_scara/cnc_scara.h"
#include "MyLibs/MyFunctions.hpp" 
#include "IoT/mqtt_syncer.h"
#include "IoT/main_mqtt.h"
#include "gobot_house.h"


StepControl controller;    // Use default settings 
Board_GobotHouse board = Board_GobotHouse();
CncScara cncScara = CncScara();
GcodeQueue gcode_queue = GcodeQueue();
MessageQueue mqtt_message_queue = MessageQueue();

GobotHouse* robot; 

void setup(){
    board.Init();
    cncScara.Init(&board);
    
    robot = &GobotHouse::getInstance();
    robot->Setup();
    robot->LinkLocalGcodeQueue_AsProducer(&gcode_queue);
    cncScara.LinkLocalGcodeQueue_AsConsumer(&gcode_queue);

    // mqtt, bridge, receiver.
    setup_mqtt_block_connect();
    // mqtt_message_queue = new MessageQueue();
    String mqtt_topic = "gobot/xROBOT_SERIAL_ID/house";
    mqtt_topic.replace("ROBOT_SERIAL_ID",String(ROBOT_SERIAL_ID));
    append_mqtt_bridge(mqtt_topic.c_str(), &mqtt_message_queue, robot); 
    setup_mqtt_on_message_receive(); 
    Serial.println("lovely bot,  GobotHouse.  setup() is done.  Good luck!");
}

void loop(){
	robot->SpinOnce();
    cncScara.SpinOnce();
    loop_mqtt();
}


#endif