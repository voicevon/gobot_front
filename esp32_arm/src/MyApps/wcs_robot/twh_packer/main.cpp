
#include "MyLibs/MyFunctions.hpp"
// #include "Mqtt/from_mqtt_client_to_remote_queue.h"
// #include "Mqtt/remote_var.h"
#include "board/board.h"
// #include "twh_packer_app.h"
// #include "robot/packer_robot.h"
#include "Mqtt/wifi_mqtt_client.h"
#include "Mqtt/mqtt_subscriber_manager.h"



#include "all_applications.h"
#ifdef I_AM_TEETH_WAREHOUSE_PACKER
// #define GCODE_MQTT_TOPIC "twh/221109/packer/gcode"  

Twh_Packer_Board board;
// GcodeQueue gcode_queue;
// MessageQueue mqtt_command_queue;

// Twh_Packer_App app;
// Twh_Packer_Robot robot;

// RemoteVar_via_Mqtt var_pick;
// RemoteVar_via_Mqtt var_pack;

void test_board(){
    #define BRIGHTNESS 11
    // board.TestLed(0, 1, 0, 0, 0);
    // board.TestLed(0, 2, 0, BRIGHTNESS, 0);
    // board.TestLed(0, 3, 0, 0, BRIGHTNESS);
    board.TestButton(0);
    Serial.println("[Info] test_board() is done.");
}

void test_robot(){
    for (int i=0; i<6;i++){
        // gcode_queue.AppendGcodeCommand("G1X190Y0");
        // gcode_queue.AppendGcodeCommand("G4S5");
    }
}

// void __connect_to_mqtt_broker(){
//     setup_wifi_mqtt();
//     while (! mqtt_is_connected){
//         delay(100);
//         Serial.print(".");
//     }
// }
void setup(){
    board.Init();
    test_board();
    // robot.Init(&board);

    // robot.LinkLocalGcodeQueue_AsConsumer(&gcode_queue);
    // app.LinkLocalGcodeQueue_AsProducer(&gcode_queue);
    // app.LinkRobot(&robot);

    // mono_remote_queue_bridge_via_mqtt_setup(GCODE_MQTT_TOPIC, &mqtt_command_queue, &app); 
    // gcode_queue.AppendGcodeCommand("G28X");   // Incase of mqtt received this command, will cause exception?
    // __connect_to_mqtt_broker();
    // test_robot();
    board.GetButton_picked()->Init_Remotable("twh/221109/packer/button/pick");
    board.GetButton_Packed()->Init_Remotable("twh/221109/packer/button/pack");
    MqttSubscriberManager::GetInstance().AddSubscriber("twh/221109/packer/led", board.GetLeds());
    Logger::Info ("Teeth Warehouse   setup() is done. ");
}


void loop(){

    // Logger::Print("Arduino loop() point ", 1);
    // app.SpinOnce();
    // Logger::Print("Aruino loop() point ", 2);
    // robot.SpinOnce();
    // robot.MySpinOnce();
    // Logger::Print("Arduino loop() point ", 3);
    // mono_remote_queue_bridge_spin_once();
    // Logger::Print("Arduino loop() point ", 4);

    board.GetButton_Packed()->SpinOnce();
    board.GetButton_picked()->SpinOnce();

}

#endif
