
#include "board/board.h"
#include "Mqtt/mqtt_subscriber_manager.h"
#include "Mqtt/remote_component/remote_var_chars.h"

#include "all_applications.h"
#ifdef I_AM_THREE_WAY_VALVE

ThreeWayValveBoard board;
RemoteVar_Chars remote_commander;

enum {
    STATE_IDLE= 1,
    STATE_GO_STRAIGHT_IDLE = 21,
    STATE_GO_STRAIGHT_SETTING = 22,
    STATE_GO_STRAIGHT_READY = 23,
    STATE_GO_STRAIGHT_INLET = 24,
    STATE_GO_STRAIGHT_OUTLET = 25,
    STATE_GO_STARIGHT_BLOCKED = 29,
    STATE_TURN_RIGHT_IDLE = 31,
    STATE_TURN_RIGHT_SETTING = 32,
    STATE_TURN_RIGHT_READY = 33,
    STATE_TURN_RIGHT_INLET = 34,
    STATE_TURN_RIGHT_OUTLET = 35,
    STATE_TURN_RIGHT_BLOCKED = 39,
} State;

void setup(){
    board.Init();

    setup_wifi_mqtt_blocking_mode();

    // app.LinkBoard(&board);

    const char* MQTT_TOPIC_FOR_TUBER_INLET = "wh221109/tube1/vvinlet1/command";
    gs_MqttSubscriberManager::Instance().AddSubscriber(MQTT_TOPIC_FOR_TUBER_INLET, &remote_commander);
    State = STATE_IDLE;
    Logger::Info ("App::loop-porter::setup() is done. ");
}


// Command from:
// 1. Remote_commander
// 2. Gpio button
void FollowCommander(){
    char* command = remote_commander.Get();
    // if (command == "go_straight"){
    if (true){
        State = STATE_GO_STRAIGHT_SETTING;
    }
    // if (command == "turn_right"){
    else if (true){
        State = STATE_TURN_RIGHT_SETTING;
        return;
    }
    // Read GPIO button
    else if (true){
        State = STATE_GO_STRAIGHT_SETTING;
        return;
    }
    else if (true){
        State = STATE_TURN_RIGHT_SETTING;
        return;
    }
}

void loop(){
    // app.SpinOnce();
    if (State == STATE_IDLE){
        FollowCommander();
    }

    // if (State == STATE_GO_STRAIGHT_IDLE){
    //     if (board.GetMotor()->is_trigered()){
    //         State = STATE_GO_STRAIGHT_READY;
    //     }
    // }

    // if (State == STATE_GO_STRAIGHT_READY){
    //     if (__inlet_sensor.is_trigered()){
    //         State = STATE_GO_STRAIGHT_INLET;
    //     }
    // }
    // if (State == STATE_GO_STRAIGHT_INLET){
    //     if (__stright_outlet_sensor.is_trigered()){
    //         State = STATE_GO_STRAIGHT_DONE;
    //     }
    // }

}

#endif
