
#include "MyLibs/MyFunctions.hpp"
#include "board/board.h"
#include "MyLibs/mqtt/wifi_mqtt_client.h"
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
    delay(10);
}

#endif
