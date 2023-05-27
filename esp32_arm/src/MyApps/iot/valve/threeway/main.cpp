
#include "board/board.h"
#include "Mqtt/mqtt_subscriber_manager.h"

#include "all_applications.h"
#ifdef I_AM_THREE_WAY_VALVE

ThreeWayValveBoard board;
RemoteVar_Int 
void setup(){
    board.Init();

    setup_wifi_mqtt_blocking_mode();

    // app.LinkBoard(&board);

    // gs_MqttSubscriberManager::Instance().AddSubscriber(MQTT_TOPIC_FOR_LEDS, &leds);
    Logger::Info ("App::loop-porter::setup() is done. ");
}

void loop(){
    // app.SpinOnce();
}

#endif
