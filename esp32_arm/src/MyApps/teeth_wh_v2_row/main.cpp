#include "all_applications.h"
#ifdef I_AM_TEETH_WAREHOUSE_V2_ROW

#define MY_ROBOT_ROW_ID 0
#define GCODE_MQTT_TOPIC "twh/221109/rows"   //report state topic =  "twh/221109/r0/state"

#include "MyLibs/MyFunctions.hpp"
#include "IoT/main_mqtt.h"
#include "MyApps/teeth_wh_v2_row/board/twh2_row_board.h"
#include "MyApps/teeth_wh_v2_row/twh2row_app.h"
#include "MyApps/teeth_wh_v2_row/robot/twh2row_robot.h"

    // char __payload_buffer[MQTT_PAYLOAD_BUFFER_COUNT_200K];

Twh2Row_Board board;
GcodeQueue gcode_queue;
MessageQueue mqtt_command_queue;

Twh2Row_App app(MY_ROBOT_ROW_ID);
Twh2Row_Robot robot;

void test_board(){
    board.Test_PositionTriggers(0);
    board.Test_Stepper(900);
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

    setup_mqtt_block_connect();
    append_mqtt_bridge(GCODE_MQTT_TOPIC, &mqtt_command_queue, &app); 
    setup_mqtt_on_message_receive(); 

    gcode_queue.AppendGcodeCommand("G28X");
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
    loop_mqtt();
    // Logger::Print("Arduino loop() point ", 4);


}

#endif
