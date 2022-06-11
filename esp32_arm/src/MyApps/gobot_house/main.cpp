#include "all_applications.h"
#ifdef I_AM_GOBOT_HOUSE

#include "MyBoards/gobot_house/board_gobot_house.h"
#include "CNC/cnc_scara/cnc_scara.h"
#include "cnc_machine.h"
#include "MyLibs/MyFunctions.hpp" 
#include "IoT/mqtt_syncer.h"
#include "IoT/main_mqtt.h"
#include "gobot_house.h"


StepControl controller;    // Use default settings 
Board_GobotHouse board;
GobotHouseMachine cncMachine;
CncScara cncScara;
GcodeQueue gcode_queue;
MessageQueue mqtt_message_queue;
GobotHouse* robot; 

void board_test();


void setup(){
    board.Init(true);
    board_test();
    
    cncMachine.Init('M');
    cncScara.Init(&board, &cncMachine);
    cncScara.LinkStepControl(&controller);
    
    robot = &GobotHouse::getInstance();
    robot->Setup();
    robot->LinkLocalGcodeQueue_AsProducer(&gcode_queue);
    cncScara.LinkLocalGcodeQueue_AsConsumer(&gcode_queue);

    // mqtt, bridge, receiver.
    setup_mqtt_block_connect();
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


#include "MyBoards/gobot_house/board_tester.h"
void board_test(){
    GobotHouse_BoardTest tester;
    tester.LinkBoard(&board);
    tester.Test_EefLoadUnload(0);
    tester.Test_AllHomers(0);
    tester.Test_Stepper(0, 'A', 300, &controller);
    tester.Test_Stepper(0, 'B', 300, &controller);

    // tester.Test_EefUpDown(100);
}

#endif