
#include "MyLibs/MyFunctions.hpp"
#include "IoT/from_mqtt_client_to_remote_queue.h"
#include "board/twh2_row_board.h"
#include "twh2row_app.h"
#include "robot/twh2row_robot.h"

#include "all_applications.h"
#ifdef I_AM_TEETH_WAREHOUSE_V2_ROW


#define MY_ROBOT_ROW_ID 1
#define MQTT_TOPIC_GCODE "twh/221109/r1/gcode"   //report state topic =  "twh/221109/r1/state"
#define MQTT_TOPIC_M408_REPORT_STATE_ON_SETUP "twh/221109/r1/state"

    // char __payload_buffer[MQTT_PAYLOAD_BUFFER_COUNT_200K];

Twh2Row_Board board;
GcodeQueue gcode_queue;
MessageQueue mqtt_command_queue;

Twh2Row_App app(MY_ROBOT_ROW_ID);
Twh2Row_Robot robot;

void test_board(){
    board.Test_PositionTriggers(0);
    board.Test_Stepper(0);
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
    // Twh2_Circleloop_ArmSolution arm;
    // FKPosition_XYZRPW input_fk;
    // input_fk.X = 123;
    // input_fk.Y = 234;
    // input_fk.Z = 12;
    // arm.Test_FK_MK_FK(&input_fk);

    // IKPosition_abgdekl input_ik;
    // input_ik.Positions[AXIS_ALPHA] = 456;
    // input_ik.Positions[AXIS_BETA] = -567;
    // arm.Test_IK_MK_IK(&input_ik);

    // MiddleKinematic input_mk;
    // input_mk.Arm_shaft_At_Y = -12.3f;
    // input_mk.Arm_Angle = -23.4f * DEG_TO_RAD;
    // arm.Test_MK_IK_MK(&input_mk);
    // arm.Test_MK_FK_MK(&input_mk);

    while (true){

    }
}



void setup(){
    board.Init();
    // test_arm();
    test_board();
    float xx = Twh2_Circleloop_Armsolution_Config().Slope_Steps_per_box();
    
    robot.Init(&board);

    robot.LinkLocalGcodeQueue_AsConsumer(&gcode_queue);
    app.LinkLocalGcodeQueue_AsProducer(&gcode_queue);
    app.LinkRobot(&robot);

    // setup_mqtt_block_connect();
    setup_mono_remote_queue_bridge_via_mqtt(MQTT_TOPIC_GCODE, &mqtt_command_queue, &app); 
    // setup_mqtt_on_message_receive(); 

    gcode_queue.AppendGcodeCommand("G28X");
    gcode_queue.AppendGcodeCommand(MQTT_TOPIC_M408_REPORT_STATE_ON_SETUP);
    // test_robot();
    Logger::Info ("Teeth Warehouse   setup() is done. ");
}


void loop(){

    // Logger::Print("Arduino loop() point ", 1);
    app.SpinOnce();
    // Logger::Print("Aruino loop() point ", 2);
    robot.SpinOnce();
    robot.MySpinOnce();
    // Logger::Print("Arduino loop() point ", 3);
    mono_remote_queue_bridge_spin_once();
    // Logger::Print("Arduino loop() point ", 4);

}

#endif