#include "all_devices.h"
#ifdef I_AM_GOBOT_CHESSBOARD

#include "gobot_chessboard.h"
#include "CNC/cnc_five_bars/cnc_five_bars.h"
#include "MyLibs/MyFunctions.hpp" 
#include "IoT/main_mqtt.h"
// #include "CNC/cnc_five_bars/board_ver1.2.h"
#include "MyBoards/cnc_dual_stepper_2201/board_ver1.2.h"
// #include "CNC/single_axis_homer.h"

// static char LOG_TAG[]= "BLE-HOUSE";
GobotChessboard* robot; 
CncFiveBars* robot_hardware;
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

void setup_robot_hardware(){
    robot = &GobotChessboard::getInstance();
    SingleAxisHomer* alpha_homer = new SingleAxisHomer(PIN_HOME_ALPHA, LOW);
    SingleAxisHomer* beta_homer = new SingleAxisHomer(PIN_HOME_BETA, LOW);
    Stepper* alpha_stepper = new Stepper(PIN_ALPHA_STEP, PIN_ALPHA_DIR); 
    Stepper* beta_stepper = new Stepper(PIN_BETA_STEP, PIN_BETA_DIR);
    robot_hardware = new CncFiveBars();
    robot_hardware->LinkHomer(alpha_homer, beta_homer);
    robot_hardware->LinkStepper(alpha_stepper, beta_stepper);
    // robot_hardware.LinkStepper();
    robot_hardware->InitRobot();
    Serial.println("[Info] setup()  is  8888888888888888");
}


void setup(){
    Serial.begin(115200);
    Serial.println("Hi Xuming, I am Gobot-Chessboard. Good luck......");
    // pinMode(PIN_ALPHA_ENABLE, OUTPUT);
    // digitalWrite(PIN_ALPHA_ENABLE,HIGH);
    // return;
    // Always init hardware first
    // Setup_RoomsSensor();
    setup_robot_hardware();

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
bool xx=true;
void loop(){
    // digitalWrite(PIN_ALPHA_ENABLE, xx);
    // delay(3000);
    // xx=!xx;
    // return;
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