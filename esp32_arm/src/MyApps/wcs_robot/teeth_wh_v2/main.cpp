

#include "MyLibs/MyFunctions.hpp"
#include "IoT/from_mqtt_client_to_remote_queue.h"
#include "board/teeth_wh_board.h"
#include "teeth_wh_app.h"
#include "robot/twh2_robot.h"
    // char __payload_buffer[MQTT_PAYLOAD_BUFFER_COUNT_200K];

#include "all_applications.h"
#ifdef I_AM_TEETH_WAREHOUSE_V2

Twh2_Board board;
GcodeQueue gcode_queue;
MessageQueue mqtt_command_queue;

Twh2_App app;
Twh2_Robot robot;

void test_board(){
    board.Test_PositionTriggers(9);
    board.Test_SingleStepper(AXIS_ALPHA, 0);
    board.Test_SingleStepper(AXIS_BETA, 0);
    board.Test_DualStepper(0);
    Serial.println("[Info] test_board() is done.");

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


void test_arm(){
    Twh2_XzArmSolution arm;
    FKPosition_XYZRPW input_fk;
    input_fk.X = 123;
    input_fk.Y = 234;
    input_fk.Z = 12;
    arm.Test_FK_MK_FK(&input_fk);

    IKPosition_abgdekl input_ik;
    input_ik.Positions[AXIS_ALPHA] = 456;
    input_ik.Positions[AXIS_BETA] = -567;
    arm.Test_IK_MK_IK(&input_ik);

    MiddleKinematic input_mk;
    input_mk.Arm_shaft_At_Y = -12.3f;
    input_mk.Arm_Angle = -23.4f * DEG_TO_RAD;
    arm.Test_MK_IK_MK(&input_mk);
    arm.Test_MK_FK_MK(&input_mk);

    while (true){

    }
}

void test_json(){
    Twh2_JsonLoader jl;
    
}

void setup(){
    board.Init();
    test_json();
    test_board();
    // test_arm();
    test_board();
    robot.Init(&board);
    robot.LinkLocalGcodeQueue_AsConsumer(&gcode_queue);
    app.LinkLocalGcodeQueue_AsProducer(&gcode_queue);
    app.LinkRobot(&robot);

    // connect_to_mqtt_broker();
    setup_mono_remote_queue_bridge_via_mqtt("twh/221109/gcode", &mqtt_command_queue, &app); 
    // setup_mqtt_on_message_receive(); 

    gcode_queue.AppendGcodeCommand("G28Z");
    gcode_queue.AppendGcodeCommand("G28X");
    // gcode_queue.AppendGcodeCommand("G28Y");
    // test_robot();
    Logger::Info ("Teeth Warehouse   setup() is done. ");
}

void loop(){

    // Logger::Print("Arduino loop() point ", 1);
    app.SpinOnce();
    // Logger::Print("Aruino loop() point ", 2);
    robot.SpinOnce();
    // Logger::Print("Arduino loop() point ", 3);
    mono_remote_queue_bridge_spin_once();
    // Logger::Print("Arduino loop() point ", 4);

}

#endif