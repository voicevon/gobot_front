
#include "MyLibs/MyFunctions.hpp"
#include "board/board.h"
#include "von/cpp/mqtt/task_mqtt.h"
#include "MyLibs/mqtt/mqtt_subscriber_manager.h"



#include "../select_app.h"
#ifdef I_AM_AQUAPONICS


AquaponicBoard board;

void setup(){
    board.Init();
    // test_board();

    setup_wifi_mqtt_blocking_mode();

    board.GetMs5837()->SetMqttPublishTopic("aquaponic/fish_tank/water_depth_mm");
    

    Logger::Info ("Twh Packer setup() is done. ");
}


void loop(){
    board.GetMs5837()->SpinOnce();
}

#endif
