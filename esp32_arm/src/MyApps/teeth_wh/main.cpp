#include "all_applications.h"
#ifdef I_AM_TEETH_WAREHOUSE

#include "MyApps/teeth_wh/board/teeth_wh_board.h"
#include "MyLibs/MyFunctions.hpp"
#include "IoT/main_mqtt.h"
#include "MyApps/teeth_wh/teeth_wh_app.h"
#include "MyApps/teeth_wh/robot/twh_robot.h"


    // char __payload_buffer[MQTT_PAYLOAD_BUFFER_COUNT_200K];


TeethWarehouse_Board board;
GcodeQueue gcode_queue;
MessageQueue mqtt_command_queue;

TeethWarehouse_App app;
TeechWarehouse_Robot robot;

void test_board(){
    board.Test_PositionTriggers(0);
    board.Test_Servo_AirPen(0);
    board.Test_Servo_AirSwitch(0);
    board.Test_VacuumPump(0);
    board.Test_SingleStepper(AXIS_ALPHA, 0);
    board.Test_SingleStepper(AXIS_BETA, 0);
    board.Test_DualStepper(0);
    Serial.println("[Info] test_board() is done.");

}

void test_robot(){
    for (int i=0; i<8;i++){
        gcode_queue.AppendGcodeCommand("G6a800b800");
        gcode_queue.AppendGcodeCommand("G6a0b0");
        gcode_queue.AppendGcodeCommand("G4S5");


        // gcode_queue.AppendGcodeCommand(String(__payload_buffer));

        // gcode_queue.AppendGcodeCommand("G1Z0");
        // gcode_queue.AppendGcodeCommand("G4S15");
        // gcode_queue.AppendGcodeCommand("G1Z100");
        // gcode_queue.AppendGcodeCommand("G4S15");

    }
    
}

void test_arm(){
    Twh_ArmSolution arm;
    FKPosition_XYZRPY input_fk;
    input_fk.X = 123;
    input_fk.Y = 234;
    input_fk.Z = 12;
    arm.Test_FK_MK_FK(&input_fk);

    IKPosition_abgdekl input_ik;
    input_ik.Positions[AXIS_ALPHA] = 456;
    input_ik.Positions[AXIS_BETA] = 567;
    arm.Test_IK_MK_IK(&input_ik);

    MiddleKinematic input_mk;
    input_mk.Arm_shaft_At_Y = 210;
    input_mk.Arm_Angle = 45 * DEG_TO_RAD;
    // arm.Test_MK_IK_MK(&input_mk);

    while (true){

    }
}
void setup(){
    board.Init(true);
    // test_arm();
    test_board();
    robot.Init(&board);
    robot.LinkLocalGcodeQueue_AsConsumer(&gcode_queue);
    app.LinkLocalGcodeQueue_AsProducer(&gcode_queue);
    app.LinkRobot(&robot);

    setup_mqtt_block_connect();
    append_mqtt_bridge("twh/221109/gcode_feed", &mqtt_command_queue, &app); 
    setup_mqtt_on_message_receive(); 

    gcode_queue.AppendGcodeCommand("G28a");
    gcode_queue.AppendGcodeCommand("G28Y");
    // gcode_queue.AppendGcodeCommand("G7R0");
    test_robot();
    Logger::Info ("Teeth Warehouse   setup() is done. ");
}

void loop(){

    // Logger::Print("Arduino loop() point ", 1);
    app.SpinOnce();
    // Logger::Print("Aruino loop() point ", 2);
    robot.SpinOnce();
    // Logger::Print("Arduino loop() point ", 3);
    loop_mqtt();
    // Logger::Print("Arduino loop() point ", 4);

}

#endif
