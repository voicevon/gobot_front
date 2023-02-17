
#include "board/board.h"
#include  "sillicon_pump_app.h"
#include "robot/sillicon_pump_robot.h"
#include "Mqtt/from_mqtt_client_to_remote_queue.h"

#include "all_applications.h"
#ifdef I_AM_SILLICON_PUMP

#define MQTT_TOPIC_GCODE "sillicon/pump/skin"   

GcodeQueue gcode_queue;
MessageQueue mqtt_command_queue;

Sillicon_Pump_Board board;
Sillicon_Pump_App app;
Sillicon_Pump_Robot robot;


void test_board(){
    // board.GetDisplayer()->Test(9999, 2);
    // board.Test_PositionTriggers(0);
    board.Test_Stepper(0);
    // Serial.println("[Info] test_board() is done  MY_ROBOT_ROW_ID=" + String(MY_ROBOT_ROW_ID));
}

void test_robot(){
    for (int i=0; i<6;i++){

        // gcode_queue.AppendGcodeCommand("G1X-90Y0");
        // gcode_queue.AppendGcodeCommand("G4S0");

        // gcode_queue.AppendGcodeCommand("G1X-90Y200");
        // gcode_queue.AppendGcodeCommand("G4S3");



        gcode_queue.AppendGcodeCommand("G1X-190Y0");
        gcode_queue.AppendGcodeCommand("G4S5");

        gcode_queue.AppendGcodeCommand("G1X0Y0");
        gcode_queue.AppendGcodeCommand("G4S5");

        gcode_queue.AppendGcodeCommand("G1X190Y0");
        gcode_queue.AppendGcodeCommand("G4S5");
    }
    
}

void setup(){
    board.Init();
    // test_board();
    robot.Init(&board);

    robot.LinkLocalGcodeQueue_AsConsumer(&gcode_queue);
    app.LinkLocalGcodeQueue_AsProducer(&gcode_queue);
    app.LinkRobot(&robot);

    mono_remote_queue_bridge_via_mqtt_setup(MQTT_TOPIC_GCODE, &mqtt_command_queue, &app); 

    gcode_queue.AppendGcodeCommand("G92X0Y0");
    gcode_queue.AppendGcodeCommand("G1X1000Y1000");
    Logger::Info ("App::loop_porter    setup() is done. ");
}


void loop(){
    app.SpinOnce();
    robot.SpinOnce();
    robot.MySpinOnce();
    mono_remote_queue_bridge_spin_once();
}

#endif
