#include "all_applications.h"
#ifdef I_AM_GOBOT_MAIN

#include "gobot_main.h"
#include "MyLibs/MyFunctions.hpp" 
#include "IoT/main_mqtt.h"
#include "board/board.h"
#include "board/robot_eef/gobot_main_eef.h"
#include "cnc/solution.h"
#include "cnc/solution_config.h"
#include "ESP32Step/src/TeensyStep.h"

Board_GobotMain board;
GobotMainCnc cnc;
GobotMain robot; 

StepControl objStepControl;    // Use default settings 
GcodeQueue gcode_queue;
MessageQueue mqtt_message_queue;


void cnc_test();
void board_test();

void setup(){
    board.Init(true);
    board.PrintOut();
    // board.LinkStepControlToCncMover(&objStepControl);

    board_test();
    cnc.Init(&board);

    robot.LinkLocalGcodeQueue_AsProducer(&gcode_queue);
    cnc.LinkLocalGcodeQueue_AsConsumer(&gcode_queue);
    cnc_test();

    // mqtt, bridge, receiver.
    setup_mqtt_block_connect();
    String mqtt_topic = "gobot/xROBOT_SERIAL_ID/arm";
    mqtt_topic.replace("ROBOT_SERIAL_ID", String(ROBOT_SERIAL_ID));
    append_mqtt_bridge(mqtt_topic.c_str(), &mqtt_message_queue, &robot); 
    setup_mqtt_on_message_receive(); 

    Logger::Info("Gobot-Main setup is done.........................................");
    for(int i=0; i< 200; i++){ loop_mqtt();}

}

uint8_t last_loaded_room;
bool xx=true;

void loop(){
    robot.SpinOnce();
    cnc.SpinOnce();
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

void cnc_test(){
    robot.Test_HomeAlpha(0);
    robot.Test_HomeBeta(0);
}

#include "board/board_tester.h"

void board_test(){
    GobotMain_BoardTest board_tester;
    board_tester.LinkBoard(&board);
    board_tester.Test_room_sensors(0);
    // board_tester.Test_StepperEnablePin(0, 'A');
    // board_tester.Test_StepperEnablePin(0, 'B');

    board_tester.Test_EefUpDown(0);
    board_tester.Test_EefLoadUnload(0);
    board_tester.Test_Eef_Full(0);
    
    // delay(5000);
    // board_tester.Test_Stepper(0, 'A', 800, &objStepControl);
    // delay(5000);
    // board_tester.Test_Stepper(0, 'B', 800, &objStepControl);
    // delay(5000);
}

#endif