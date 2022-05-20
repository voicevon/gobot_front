#include "all_devices.h"
#ifdef I_AM_GOBOT_CHESSBOARD

#include "gobot_chessboard.h"
#include "gobot_chessboard_hw.h"
#include "MyLibs/MyFunctions.hpp" 
#include "IoT/main_mqtt.h"
// #include "Robot/Commu/CommuBleGattServer.h"
#include "board_ver1.2.h"
#include "Robot/single_axis_homer.h"

// static char LOG_TAG[]= "BLE-HOUSE";
GobotChessboard* robot; 
GobotChessboardHardware* robot_hardware;
GcodeQueue* gcode_queue;
// RobotAction action;
MessageQueue* mqtt_message_queue;



uint8_t PIN_ROOMS[] = {PIN_SENSOR_ROOM_0,
                        PIN_SENSOR_ROOM_1,
                        PIN_SENSOR_ROOM_2,
                        PIN_SENSOR_ROOM_3,
                        PIN_SENSOR_ROOM_4,
                        PIN_SENSOR_ROOM_5,
                        PIN_SENSOR_ROOM_6,
                        PIN_SENSOR_ROOM_7
                        };
void Setup_RoomsSensor(){
    for(int i=0;i<8;i++){
        pinMode(PIN_ROOMS[i],INPUT_PULLUP);
    }
}
char ReadRoomsSensor(){
    char result = 0;
    uint8_t p;
    for (int i=0; i<8; i++){
        p = digitalRead(PIN_ROOMS[i]);
        result += (p<<i);
    }
    return result;
}

void setup(){
    Serial.begin(115200);
    Serial.println("Hi Xuming, I am Gobot-Chessboard. Good luck......");
    // Setup_RoomsSensor();
    // Always init hardware first
    delay(200);
    robot = &GobotChessboard::getInstance();
    Serial.println("[Info] setup()  is  111111111111111");
    SingleAxisHomer* alpha_homer = new SingleAxisHomer(PIN_HOME_ALPHA, LOW);
    Serial.println("[Info] setup()  is  222222222222222");
    SingleAxisHomer* beta_homer = new SingleAxisHomer(PIN_HOME_BETA, LOW);
    Serial.println("[Info] setup()  is  333333333333333");
    Stepper* alpha_stepper = new Stepper(PIN_ALPHA_STEP, PIN_ALPHA_DIR); 
    Serial.println("[Info] setup()  is  444444444444444");
    Stepper* beta_stepper = new Stepper(PIN_BETA_STEP, PIN_BETA_DIR);
    Serial.println("[Info] setup()  is  555555555555555");
    robot_hardware = new GobotChessboardHardware();
    Serial.println("[Info] setup()  is  666666666666666");
    robot_hardware->LinkHomer(alpha_homer, beta_homer);
    robot_hardware->LinkStepper(alpha_stepper, beta_stepper);
    Serial.println("[Info] setup()  is  777777777777777");
    // robot_hardware.LinkStepper();
    robot_hardware->InitRobot();
    Serial.println("[Info] setup()  is  8888888888888888");

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

    Serial.print("\nGobot-Chessboard setup is done..........");
}

char last_rooms_sensor;
void loop(){
    robot->SpinOnce();
    robot_hardware->SpinOnce();
    loop_mqtt();
    return;
    char rooms_sensor = ReadRoomsSensor();
    if (last_rooms_sensor != rooms_sensor){
        String topic = "gobot/xROBOT_SERIAL_ID/rooms";
        topic.replace("ROBOT_SERIAL_ID", String(ROBOT_SERIAL_ID));
        mqttClient.publish(topic.c_str(),2,true, &rooms_sensor);
        last_rooms_sensor = rooms_sensor;
    }
}
#endif