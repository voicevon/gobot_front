#include "all_applications.h"
#ifdef I_AM_TEETH_WAREHOUSE_V4_SHIP_OUT

#define MY_ROBOT_ROW_ID 0
#define GCODE_MQTT_TOPIC "twh/221109/shipout"  

#include "MyLibs/MyFunctions.hpp"
#include "IoT/from_mqtt_client_to_remote_queue.h"
// #include "IoT/main_mqtt.h"
#include "board/twh4_ship_out_board.h"
#include "twh4_ship_out_app.h"
#include "robot/twh4_ship_out_robot.h"

    // char __payload_buffer[MQTT_PAYLOAD_BUFFER_COUNT_200K];

Twh4_ShipOut_Board board;
GcodeQueue gcode_queue;
MessageQueue mqtt_command_queue;

Twh4_ShipOut_App app(MY_ROBOT_ROW_ID);
Twh4_ShipOut_Robot robot;

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
    setup_mono_remote_queue_bridge_via_mqtt(GCODE_MQTT_TOPIC, &mqtt_command_queue, &app); 
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
    GpioButton* button_withdraw = board.GetButton_Withdraw();
    button_withdraw->SpinOnce();
    if (button_withdraw->IsToPressed()){
        gcode_queue.AppendGcodeCommand("M408");
    }
    GpioButton* button_shipout = board.GetButton_Shipout();
    if (button_shipout->IsToPressed()){
        gcode_queue.AppendGcodeCommand("M408");
    }

}

#endif
