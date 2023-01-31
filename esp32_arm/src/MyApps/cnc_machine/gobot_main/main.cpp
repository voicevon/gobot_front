

#include "board/board.h"
#include "board/robot_eef/gobot_main_eef.h"
#include "FastAccelStepper.h"
#include "robot/gobot_main_robot.h"
#include "gobot_main_app.h"
#include "MyLibs/MyFunctions.hpp" 
#include "Mqtt/from_mqtt_client_to_remote_queue.h"

#include "all_applications.h"
#ifdef I_AM_GOBOT_MAIN
GobotMain_Board board;
GobotMainRobot robot; 
GobotMain_App app;

// StepControl objStepControl;    // Use default settings 
GcodeQueue gcode_queue;
MessageQueue mqtt_message_queue;


void cnc_test();
void board_test();

void setup(){
    board.Init();
    robot.Init(&board);

    app.LinkLocalGcodeQueue_AsProducer(&gcode_queue);
    robot.LinkLocalGcodeQueue_AsConsumer(&gcode_queue);

    // mqtt, bridge, receiver.
    // connect_to_mqtt_broker();
    String mqtt_topic = "gobot/xROBOT_SERIAL_ID/arm";
    mqtt_topic.replace("ROBOT_SERIAL_ID", String(ROBOT_SERIAL_ID));
    mono_remote_queue_bridge_via_mqtt_setup(mqtt_topic.c_str(), &mqtt_message_queue, &app); 
    // setup_mqtt_on_message_receive(); 

    Logger::Info("Gobot-Main setup is done.........................................");
    for(int i=0; i< 200; i++){ mono_remote_queue_bridge_spin_once();}
    cnc_test();

}

uint8_t last_loaded_room;
bool xx=true;

void loop(){
    robot.SpinOnce();
    app.SpinOnce();
    mono_remote_queue_bridge_spin_once();
    return;
    
    uint8_t loadded_room = board.GetLoadedRoom();
    char c_room_sensor = '?';
    if (last_loaded_room != loadded_room){
        String topic = "gobot/xROBOT_SERIAL_ID/rooms";
        topic.replace("ROBOT_SERIAL_ID", String(ROBOT_SERIAL_ID));
        g_mqttClient.publish(topic.c_str(),2,true, &c_room_sensor);
        last_loaded_room = loadded_room;
    }
}

void cnc_test(){
    // robot.Test_HomeAlpha(0);
    // robot.Test_HomeBeta(0);
    // robot.Test_PickPlace(99);
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