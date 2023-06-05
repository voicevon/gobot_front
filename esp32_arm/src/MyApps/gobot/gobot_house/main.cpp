

#include "board_2206/board_gobot_house.h"
#include "gobot_house_app.h"
#include "robot/gobot_house_robot.h"
#include "MyLibs/MyFunctions.hpp" 
#include "FastAccelStepper.h"

#include "../select_app.h"
#ifdef I_AM_GOBOT_HOUSE

Board_GobotHouse_2206 board = Board_GobotHouse_2206();
GcodeQueue gcode_queue;
// MessageQueue mqtt_message_queue;
GobotHouseApp* app; 
GobotHouseRobot robot;

#define MQTT_TOPIC_GCODE "gobot1234/house/gcode"

// void board_test();
// void cnc_test();

// #define ROBOT_SERIAL_ID 111
// #include "board_2206/board_tester.h"

void board_test(){
    // GobotHouse_2206_BoardTest tester = GobotHouse_2206_BoardTest(&stepControl);
    // tester.LinkBoard(&board);
    // tester.LinkStepControl(&stepcontrol);
    // tester.Test_EefLoadUnload(0);
    // // tester.Test_AllHomers(0);
    // tester.Test_StepperDriver_OnAlpha(0);
    // tester.Test_ServoDriver_OnBeta(0);

}



void setup(){
    board.Init();
    // board_test();
    
    // cncMachine.Init('M');
    // cnc.Init(&board);

    app = &GobotHouseApp::getInstance();
    app->Setup();
    app->Link_Mqtt_to_GcodeQueue(MQTT_TOPIC_GCODE, &gcode_queue);
    // robot.LinkLocalGcodeQueue_AsConsumer(&gcode_queue);
    // cnc_test();
    // robot->__Home();


    // mqtt, bridge, receiver.
    // connect_to_mqtt_broker();
    String mqtt_topic = "gobot/xROBOT_SERIAL_ID/house";
    mqtt_topic.replace("ROBOT_SERIAL_ID",String(ROBOT_SERIAL_ID));
    // mono_remote_queue_bridge_via_mqtt_setup(mqtt_topic.c_str(), &mqtt_message_queue, app); 
    // setup_mqtt_on_message_receive(); 
    Serial.println("lovely bot,  GobotHouse.  setup() is done.  Good luck!");
    // board.GetEef()->Run(EEF_CODE_UNLOAD);
}

void loop(){
    board.GetActuaorBeta('B')->SpinOnce();
	robot.SpinOnce();
    app->SpinOnce();
    // cnc.SpinOnce();
    // mono_remote_queue_bridge_spin_once();
}






#endif