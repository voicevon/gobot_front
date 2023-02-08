
#include "MyLibs/MyFunctions.hpp"
#include "Mqtt/from_mqtt_client_to_remote_queue.h"
#include "Mqtt/remote_var.h"
#include "board/board.h"
#include "twh_packer_app.h"
#include "robot/packer_robot.h"


#include "all_applications.h"
#ifdef I_AM_TEETH_WAREHOUSE_PACKER
#define GCODE_MQTT_TOPIC "twh/221109/packer/gcode"  

Twh_Packer_Board board;
GcodeQueue gcode_queue;
MessageQueue mqtt_command_queue;

Twh_Packer_App app;
Twh_Packer_Robot robot;

RemoteVar_via_Mqtt var_pick;
RemoteVar_via_Mqtt var_pack;

void test_board(){
    #define BRIGHTNESS 11
    board.TestLed(0, 1, 0, 0, 0);
    board.TestLed(0, 2, 0, BRIGHTNESS, 0);
    board.TestLed(0, 3, 0, 0, BRIGHTNESS);
    board.TestButton(0);
    Serial.println("[Info] test_board() is done.");
}

void test_robot(){
    for (int i=0; i<6;i++){
        // gcode_queue.AppendGcodeCommand("G1X190Y0");
        // gcode_queue.AppendGcodeCommand("G4S5");
    }
}

void setup(){
    board.Init();
    test_board();
    robot.Init(&board);

    robot.LinkLocalGcodeQueue_AsConsumer(&gcode_queue);
    app.LinkLocalGcodeQueue_AsProducer(&gcode_queue);
    app.LinkRobot(&robot);

    // setup_mqtt_block_connect();
    mono_remote_queue_bridge_via_mqtt_setup(GCODE_MQTT_TOPIC, &mqtt_command_queue, &app); 
    // setup_mqtt_on_message_receive(); 

    gcode_queue.AppendGcodeCommand("G28X");   // Incase of mqtt received this command, will cause exception?
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

    
    // check button: is from unpress to pressed
    Button_Gpio* button_picked = board.GetButton_picked();
    button_picked->SpinOnce();
    if (button_picked->IsToPressed()){
        var_pick.Set("pressed");
        // gcode_queue.AppendGcodeCommand("M408");
    }
    Button_Gpio* button_shipout = board.GetButton_Packed();
    if (button_shipout->IsToPressed()){
        gcode_queue.AppendGcodeCommand("M408");
    }

}

#endif