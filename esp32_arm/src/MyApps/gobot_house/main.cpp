#include "all_applications.h"
#ifdef I_AM_GOBOT_HOUSE

// #include "MyBoards/gobot_house/board_gobot_house_22.h"
#include "board/board_gobot_house.h"
// #include "CNC/solution/cnc_scara/cnc_scara.h"
#include "board_2206/mechanic/cnc_solution.h"
// #include "cnc_machine.h"
#include "board/cnc_machine.h"
#include "MyLibs/MyFunctions.hpp" 
#include "IoT/mqtt_syncer.h"
#include "IoT/main_mqtt.h"
#include "gobot_house.h"


StepControl controller;    // Use default settings 
Board_GobotHouse board;
// GobotHouseMachine cncMachine;
GobotHouse2206Cnc cnc;
GcodeQueue gcode_queue;
MessageQueue mqtt_message_queue;
GobotHouse* robot; 

void board_test();
void cnc_test();


void setup(){
    board.Init(true);
    board_test();
    
    // cncMachine.Init('M');
    cnc.Init(&board);
    // cnc.LinkStepControl(&controller);
    
    robot = &GobotHouse::getInstance();
    robot->Setup();
    robot->LinkLocalGcodeQueue_AsProducer(&gcode_queue);
    cnc.LinkLocalGcodeQueue_AsConsumer(&gcode_queue);

    // mqtt, bridge, receiver.
    setup_mqtt_block_connect();
    String mqtt_topic = "gobot/xROBOT_SERIAL_ID/house";
    mqtt_topic.replace("ROBOT_SERIAL_ID",String(ROBOT_SERIAL_ID));
    append_mqtt_bridge(mqtt_topic.c_str(), &mqtt_message_queue, robot); 
    setup_mqtt_on_message_receive(); 
    Serial.println("lovely bot,  GobotHouse.  setup() is done.  Good luck!");
    cnc_test();
}

void loop(){
	robot->SpinOnce();
    cnc.SpinOnce();
    loop_mqtt();
}


// #include "MyBoards/gobot_house/board_tester.h"
#include "board/board_tester.h"
void board_test(){
    GobotHouse_BoardTest tester;
    tester.LinkBoard(&board);
    tester.Test_EefLoadUnload(0);
    tester.Test_AllHomers(0);
    // tester.Test_Stepper(0, 'A', 300, &controller);
    // tester.Test_Stepper(0, 'B', 300, &controller);
}

void cnc_test(){
    robot->Test_Alpha(0);
    robot->Test_Beta(0);
}

#endif