
#include "board/board.h"
#include "von/utility/mqtt/subscriber/mqtt_subscriber_manager.h"
#include "MyLibs/mqtt/remote_component/remote_var_chars.h"


#include "../select_app.h"
#ifdef I_AM_DENTURE_STATION_MOUNT_TEETH

#define MY_WORKSTATION_ID       "101"
#define WARHOUSE_ID             "221109"



ThreeWayValveBoard board;


void setup(){
    #define MQTT_TOPIC_OF_ORDER_REQUEST "wh" WARHOUSE_ID "/station"  MY_WORKSTATION_ID  "/order"
    // setup_wifimanager();

    board.Init();

    // setup_wifi_mqtt_blocking_mode();

    // const char* MQTT_TOPIC_FOR_TUBER_INLET = "wh221109/tube1/vvinlet1/command";
    // gs_MqttSubscriberManager::Instance().AddSubscriber(MQTT_TOPIC_FOR_TUBER_INLET, &remote_commander);
    // Logger::Info ("App::loop-porter::setup() is done. ");
    board.TestPositionTriggers(9999);

}


// Command from:
// 1. Remote_commander
// 2. Gpio button
void FollowCommander(){
    // char* command = remote_commander.Get();
    // // if (command == "go_straight"){
    // if (true){
    //     board.GetValve()->SetMode_GoStraight();
    // }
    // // if (command == "turn_right"){
    // else if (true){
    //     board.GetValve()->SetMode_TurnRight();
    // }
    // // Read GPIO button
    // else if (true){
    //     board.GetValve()->SetMode_GoStraight();
    // }
    // else if (true){
    //     board.GetValve()->SetMode_TurnRight();
    // }
}

void loop(){
    // if (board.GetValve()->GetState() == ThreeWayValve::EnumState::STATE_IDLE){
    //     FollowCommander();
    // }
    // board.GetValve()->SpinOnce_Statemachine();

    // if (board.GetBarcodeReader()->Read()){
    //     char* order_id = board.GetBarcodeReader()->GetBuffer();
    //     // publish order request.
    //     g_mqttClient.publish(MQTT_TOPIC_OF_ORDER_REQUEST, 2, true, order_id);
    // }

}

#endif
