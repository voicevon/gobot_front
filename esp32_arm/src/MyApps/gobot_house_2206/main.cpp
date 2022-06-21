#include "all_applications.h"
#ifdef I_AM_GOBOT_HOUSE_2206

#include "MyBoards/gobot_house_2206/board_gobot_house.h"
#include "CNC/solution/cnc_scara/cnc_scara.h"
// #include "cnc_machine.h"
#include "MyLibs/MyFunctions.hpp" 
#include "IoT/mqtt_syncer.h"
#include "IoT/main_mqtt.h"
#include "gobot_house.h"

// StepControl controller;    // Use default settings 
Board_GobotHouse_2206 board;
// GobotHouseMachine_2206 cncMachine;
CncScara cncScara;
GcodeQueue gcode_queue;
MessageQueue mqtt_message_queue;

GobotHouse_2206* robot; 

void board_test();
void cnc_test();



void setup(){
    board.Init(true);
    board_test();
    
    // cncMachine.Init('M');
    cncScara.Init(&board);

    robot = &GobotHouse_2206::getInstance();
    robot->Setup();
    robot->LinkLocalGcodeQueue_AsProducer(&gcode_queue);
    cncScara.LinkLocalGcodeQueue_AsConsumer(&gcode_queue);
    cnc_test();
    // robot->__Home();


    // mqtt, bridge, receiver.
    setup_mqtt_block_connect();
    String mqtt_topic = "gobot/xROBOT_SERIAL_ID/house";
    mqtt_topic.replace("ROBOT_SERIAL_ID",String(ROBOT_SERIAL_ID));
    append_mqtt_bridge(mqtt_topic.c_str(), &mqtt_message_queue, robot); 
    setup_mqtt_on_message_receive(); 
    Serial.println("lovely bot,  GobotHouse.  setup() is done.  Good luck!");
    board.GetEef()->Run(EEF_CODE_UNLOAD);
}

void loop(){
    board.GetActuaorBeta('B')->SpinOnce();
	robot->SpinOnce();
    cncScara.SpinOnce();
    loop_mqtt();
}


#include "MyBoards/gobot_house_2206/board_tester.h"
void board_test(){
    GobotHouse_2206_BoardTest tester;
    tester.LinkBoard(&board);
    tester.Test_EefLoadUnload(0);
    tester.Test_AllHomers(0);
    tester.Test_ServoDriver_OnBeta(0);

}

void cnc_test(){
    Serial.println("[Info] Cnc teset is started.");
    robot->Test_Beta(0);
    robot->Test_Alpha(0);
    robot->Test_FollowJig(5);

    // robot->__Home();
    robot->Test_MoveStone_FromRoomToHead(0, 0);
}

#endif