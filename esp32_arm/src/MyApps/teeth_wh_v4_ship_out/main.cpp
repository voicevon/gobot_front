#include "all_applications.h"
#ifdef I_AM_TEETH_WAREHOUSE_V4_SHIP_OUT

#define MY_ROBOT_ROW_ID 0
#define GCODE_MQTT_TOPIC "twh/221109/delivery"  

#include "MyLibs/MyFunctions.hpp"
#include "IoT/main_mqtt.h"
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
    board.TestLed(0, 1);
    board.TestLed(555, 2);
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

    
    // check button: is from unpress to pressed
    GpioButton* button = board.GetButton();
    button->SpinOnce();
    if (button->IsToPressed()){
        gcode_queue.AppendGcodeCommand("M408");
    }

}

#endif
