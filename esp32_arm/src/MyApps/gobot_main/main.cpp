#include "all_applications.h"
// #include "all_devices.h"
#ifdef I_AM_GOBOT_MAIN

#include "gobot_chessboard.h"
#include "CNC/cnc_five_bars/cnc_five_bars.h"
#include "cnc_machine.h"
#include "MyLibs/MyFunctions.hpp" 
#include "IoT/main_mqtt.h"
#include "MyBoards/gobot_main/board_gobot_main.h"

StepControl controller;    // Use default settings 
Board_GobotMain board;
GobotMainMachine cncMachine;
CncFiveBars cncFiveBar;
GobotChessboard robot; 
GcodeQueue gcode_queue;
MessageQueue mqtt_message_queue;


// #include "MyBoards/unit_test/unit_test_cnc.h"
#include "MyBoards/gobot_main/unit_test.h"

void unit_test(){
    GobotMain_UnitTest* tester = new GobotMain_UnitTest();
    tester->LinkBoard(&board);
    tester->Test_AllHomers(100);
    tester->Test_Stepper(10, 'A', 300, &controller);
    tester->Test_Stepper(10, 'B', 300, &controller);
    tester->Test_room_sensors(100);
}

void setup(){
    board.Init(true);
    unit_test();
    
    cncMachine.Init('S');  //Slow moving
    cncFiveBar.Init(&board, &cncMachine);

    robot.LinkLocalGcodeQueue_AsProducer(&gcode_queue);
    cncFiveBar.LinkLocalGcodeQueue_AsConsumer(&gcode_queue);

    // mqtt, bridge, receiver.
    setup_mqtt_block_connect();
    String mqtt_topic = "gobot/xROBOT_SERIAL_ID/arm";
    mqtt_topic.replace("ROBOT_SERIAL_ID", String(ROBOT_SERIAL_ID));
    append_mqtt_bridge(mqtt_topic.c_str(), &mqtt_message_queue, &robot); 
    setup_mqtt_on_message_receive(); 

    Serial.print("\nGobot-Chessboard setup is done..........");
}

uint8_t last_loaded_room;
bool xx=true;

void loop(){
    robot.SpinOnce();
    cncFiveBar.SpinOnce();
    loop_mqtt();
    return;
    uint8_t loadded_room = board.GetLoadedRoom();
    char c_room_sensor = '?';
    if (last_loaded_room != loadded_room){
        String topic = "gobot/xROBOT_SERIAL_ID/rooms";
        topic.replace("ROBOT_SERIAL_ID", String(ROBOT_SERIAL_ID));
        mqttClient.publish(topic.c_str(),2,true, &c_room_sensor);
        last_loaded_room = loadded_room;
    }
}
#endif