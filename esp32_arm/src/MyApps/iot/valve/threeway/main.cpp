
#include "board/board.h"
#include "Mqtt/mqtt_subscriber_manager.h"
#include "Mqtt/remote_component/remote_var_chars.h"

#include "all_applications.h"
#ifdef I_AM_THREE_WAY_VALVE

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

}

#endif
