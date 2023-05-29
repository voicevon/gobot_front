
#include "board/board.h"
#include "MyLibs/mqtt/mqtt_subscriber_manager.h"
// #include "Mqtt/remote_component/remote_var_chars.h"
#include "MyLibs/mqtt/remote_component/remote_var_chars.h"

#include "all_applications.h"
#ifdef I_AM_DENTURE_STATION_MOUNT_TEETH
#define MY_WORKSTATION_ID "101"
#define WARHOUSE_ID "221109"
#define ORDER "/order"
#define MQTT_TOPIC_OF_ORDER_REQUEST "wh" WARHOUSE_ID "/STATION"  MY_WORKSTATION_ID  "/order"
// #define MQTT_TOPIC_OF_ORDER_REQUEST "wh221109/station101/order"
ThreeWayValveBoard board;
RemoteVar_Chars remote_commander;

void setup(){
    board.Init();
    setup_wifi_mqtt_blocking_mode();

    const char* MQTT_TOPIC_FOR_TUBER_INLET = "wh221109/tube1/vvinlet1/command";
    gs_MqttSubscriberManager::Instance().AddSubscriber(MQTT_TOPIC_FOR_TUBER_INLET, &remote_commander);
    Logger::Info ("App::loop-porter::setup() is done. ");
}


// Command from:
// 1. Remote_commander
// 2. Gpio button
void FollowCommander(){
    char* command = remote_commander.Get();
    // if (command == "go_straight"){
    if (true){
        board.GetValve()->SetMode_GoStraight();
    }
    // if (command == "turn_right"){
    else if (true){
        board.GetValve()->SetMode_TurnRight();
    }
    // Read GPIO button
    else if (true){
        board.GetValve()->SetMode_GoStraight();
    }
    else if (true){
        board.GetValve()->SetMode_TurnRight();
    }
}

void loop(){
    if (board.GetValve()->GetState() == ThreeWayValve::EnumState::STATE_IDLE){
        FollowCommander();
    }
    board.GetValve()->SpinOnce_Statemachine();

    if (board.GetBarcodeReader()->Read()){
        char* order_id = board.GetBarcodeReader()->GetBuffer();
        // publish order request.
        g_mqttClient.publish(MQTT_TOPIC_OF_ORDER_REQUEST, 2, true, order_id);
    }

}

#endif
