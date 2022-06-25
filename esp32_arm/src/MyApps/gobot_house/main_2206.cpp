#include "all_applications.h"
#ifdef I_AM_GOBOT_HOUSE_2206

#include "board_2206/board_gobot_house.h"
// #include "CNC/solution/cnc_scara/cnc_scara.h"
#include "board/mechanic/cnc_solution.h"
#include "MyLibs/MyFunctions.hpp" 
#include "IoT/mqtt_syncer.h"
#include "IoT/main_mqtt.h"
#include "gobot_house.h"


#include "ESP32Step/src/TeensyStep.h"
StepControl stepControl;

Board_GobotHouse_2206 board = Board_GobotHouse_2206(&stepControl);
GobotHouseCnc cnc;
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

    robot = &GobotHouse::getInstance();
    robot->Setup();
    robot->LinkLocalGcodeQueue_AsProducer(&gcode_queue);
    cnc.LinkLocalGcodeQueue_AsConsumer(&gcode_queue);
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
    cnc.SpinOnce();
    loop_mqtt();
}


#include "board_2206/board_tester.h"
void board_test(){
    GobotHouse_2206_BoardTest tester = GobotHouse_2206_BoardTest(&stepControl);
    tester.LinkBoard(&board);
    // tester.LinkStepControl(&stepcontrol);
    tester.Test_EefLoadUnload(0);
    tester.Test_AllHomers(0);
    tester.Test_StepperDriver_OnAlpha(0);
    tester.Test_ServoDriver_OnBeta(0);

}

void cnc_test(){
    Serial.println("[Info] Cnc teset is started.");
    robot->__Home();
    robot->Test_Beta(0);
    robot->Test_Alpha(0);
    robot->Test_FollowJig(0);

    robot->Test_MoveStone_FromRoomToHead(8, 0);
}

#endif