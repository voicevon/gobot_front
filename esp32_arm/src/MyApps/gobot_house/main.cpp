#include "all_devices.h"
#ifdef I_AM_GOBOT_HOUSE

#include "gobot_house.h"
#include "gobot_house_hw.h"
#include "MyLibs/MyFunctions.hpp" 
#include "IoT/mqtt_syncer.h"
#include "IoT/main_mqtt.h"

GobotHouse* robot; 
GobotHouseHardware* robot_hardware;
GcodeQueue* gcode_queue;
MessageQueue* mqtt_message_queue;

void setup(){
    Serial.begin(115200);
    Serial.println("Hi Xuming, I am your lovely bot,  GobotHouse. ");
    robot = &GobotHouse::getInstance();
    robot_hardware = &GobotHouseHardware::getInstance();
    gcode_queue = new GcodeQueue();
    robot->Setup();
    robot->LinkLocalGcodeQueue_AsProducer(gcode_queue);
    robot_hardware->LinkLocalGcodeQueue_AsConsumer(gcode_queue);

    // mqtt, bridge, receiver.
    setup_mqtt_block_connect();
    mqtt_message_queue = new MessageQueue();
    String mqtt_topic = "gobot/xROBOT_SERIAL_ID/house";
    mqtt_topic.replace("SERIAL_ID",String(ROBOT_SERIAL_ID));
    append_mqtt_bridge(mqtt_topic.c_str(), mqtt_message_queue, robot); 
    setup_mqtt_on_message_receive(); 
    Serial.println("lovely bot,  GobotHouse.  setup() is done.  Good luck!");
}

void loop(){
	robot->SpinOnce();
    robot_hardware->SpinOnce();
    loop_mqtt();
}


#endif