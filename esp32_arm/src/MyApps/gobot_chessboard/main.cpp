#include "all_devices.h"
#ifdef I_AM_GOBOT_CHESSBOARD

#include "gobot_chessboard.h"
#include "CNC/cnc_five_bars/cnc_five_bars.h"
#include "MyLibs/MyFunctions.hpp" 
#include "IoT/main_mqtt.h"
#include "MyBoards/gobot_main/board_gobot_main.h"


Board_GobotMain board = Board_GobotMain();
CncFiveBars cncFiveBar;

GobotChessboard* robot; 
GcodeQueue* gcode_queue;
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
    board.Init();
    cncFiveBar.Init(&board);

    robot = &GobotChessboard::getInstance();
    gcode_queue = new GcodeQueue();
    robot->LinkLocalGcodeQueue_AsProducer(gcode_queue);
    cncFiveBar.LinkLocalGcodeQueue_AsConsumer(gcode_queue);

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
    robot->SpinOnce();
    cncFiveBar.SpinOnce();
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