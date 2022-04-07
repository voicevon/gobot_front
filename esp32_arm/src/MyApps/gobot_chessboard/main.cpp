#include "all_devices.h"
#ifdef I_AM_GOBOT_CHESSBOARD

#include "gobot_chessboard.h"
#include "gobot_chessboard_hw.h"
#include "MyLibs/MyFunctions.hpp" 
#include "IoT/main_mqtt.h"
// #include "Robot/Commu/CommuBleGattServer.h"


// static char LOG_TAG[]= "BLE-HOUSE";
GobotChessboard* robot; 
GobotChessboardHardware* robot_hardware;
GcodeQueue* gcode_queue;
// RobotAction action;
MessageQueue* mqtt_message_queue;

void setup(){
    Serial.begin(115200);
    Serial.println("Hi Xuming, I am Gobot-Chessboard. Good luck......");
    setup_mqtt_block_connect();

    // ble.Init();
    // Serial.println("BLE is ok....");   
    gcode_queue = new GcodeQueue();

    robot = &GobotChessboard::getInstance();
    robot_hardware = new GobotChessboardHardware();
    robot_hardware->InitRobot();
    robot->LinkLocalGcodeQueue_AsProducer(gcode_queue);
    robot_hardware->LinkLocalGcodeQueue_AsConsumer(gcode_queue);

    mqtt_message_queue = new MessageQueue();
    append_mqtt_link("gobot/x2134/arm", mqtt_message_queue, robot);  

    // mybot->Calibrate(1);
    // mybot->ParkArms(true);
    // Begin_WifiRabbitMqtt();
    Serial.print("\nGobot-Chessboard setup is done..........");
}

void loop(){
    robot->SpinOnce();
    robot_hardware->SpinOnce();
    loop_mqtt();
}
#endif