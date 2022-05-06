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


#define PIN_SENSOR_ROOM_0 11
#define PIN_SENSOR_ROOM_1 11
#define PIN_SENSOR_ROOM_2 11
#define PIN_SENSOR_ROOM_3 11
#define PIN_SENSOR_ROOM_4 11
#define PIN_SENSOR_ROOM_5 11
#define PIN_SENSOR_ROOM_6 11
#define PIN_SENSOR_ROOM_7 11

uint8_t PIN_ROOMS[] = {PIN_SENSOR_ROOM_0,
                        PIN_SENSOR_ROOM_1,
                        PIN_SENSOR_ROOM_2,
                        PIN_SENSOR_ROOM_3,
                        PIN_SENSOR_ROOM_4,
                        PIN_SENSOR_ROOM_5,
                        PIN_SENSOR_ROOM_6,
                        PIN_SENSOR_ROOM_7
                        };
char ReadRoomsSensor(){
    char result = 0;
    uint8_t p;
    for (int i; i<8; i++){
        p = digitalRead(PIN_ROOMS[i]);
        result += (p<<i);
    }
    return result;
}

void setup(){
    Serial.begin(115200);
    Serial.println("Hi Xuming, I am Gobot-Chessboard. Good luck......");
    // Always init hardware first
    robot = &GobotChessboard::getInstance();
    robot_hardware = new GobotChessboardHardware();
    robot_hardware->InitRobot();

    // ble.Init();
    // Serial.println("BLE is ok....");   
    gcode_queue = new GcodeQueue();
    robot->LinkLocalGcodeQueue_AsProducer(gcode_queue);
    robot_hardware->LinkLocalGcodeQueue_AsConsumer(gcode_queue);


    // mqtt, bridge, receiver.
    setup_mqtt_block_connect();
    mqtt_message_queue = new MessageQueue();
    String mqtt_topic = "gobot/xROBOT_SERIAL_ID/arm";
    mqtt_topic.replace("ROBOT_SERIAL_ID", String(ROBOT_SERIAL_ID));
    append_mqtt_bridge(mqtt_topic.c_str(), mqtt_message_queue, robot); 
    setup_mqtt_on_message_receive(); 


    // mybot->Calibrate(1);
    // mybot->ParkArms(true);
    // Begin_WifiRabbitMqtt();
    Serial.print("\nGobot-Chessboard setup is done..........");
}

char last_rooms_sensor;
void loop(){
    robot->SpinOnce();
    robot_hardware->SpinOnce();
    loop_mqtt();

    char rooms_sensor = ReadRoomsSensor();
    if (last_rooms_sensor != rooms_sensor){
        mqttClient.publish("gobot/x2134/rooms",2,true, &rooms_sensor);
        last_rooms_sensor = rooms_sensor;
    }
}
#endif