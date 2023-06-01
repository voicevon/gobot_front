
#include "board/board.h"
#include  "sillicon_pump_app.h"
#include "robot/sillicon_pump_robot.h"


#include "all_applications.h"
#ifdef I_AM_SILLICON_PUMP

// #define MQTT_TOPIC_GCODE "silicon/pump/skin"   
#define MQTT_TOPIC_GCODE "silicon/pump/foam"   

GcodeQueue gcode_queue;
// MessageQueue mqtt_command_queue;

Silicon_Pump_Board board;
Silicon_Pump_App app;
Silicon_Pump_Robot robot;


void test_board(){
    board.Test_Stepper(0);
    // Serial.println("[Info] test_board() is done  MY_ROBOT_ROW_ID=" + String(MY_ROBOT_ROW_ID));
}



void setup(){
    board.Init();
    // test_board();
    robot.Init(&board);

    // robot.LinkLocalGcodeQueue_AsConsumer(&gcode_queue);
    app.Link_Mqtt_to_GcodeQueue(MQTT_TOPIC_GCODE, &gcode_queue);
    // app.LinkLocalGcodeQueue_AsProducer(&gcode_queue);
    app.LinkRobot(&robot);

    // mono_remote_queue_bridge_via_mqtt_setup(MQTT_TOPIC_GCODE, &mqtt_command_queue, &app); 

    gcode_queue.AppendGcodeCommand("G92X0Y0");
    // gcode_queue.AppendGcodeCommand("G1X1000Y1000");
    Logger::Info ("App::loop_porter    setup() is done. ");
    board.GetStateLed()->TurnOn();
}


void loop(){
    app.SpinOnce();
    robot.SpinOnce();
    robot.MySpinOnce();
    // mono_remote_queue_bridge_spin_once();
}

#endif
