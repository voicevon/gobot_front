
#include "MyLibs/MyFunctions.hpp"
#include "board/board.h"
#include "Mqtt/wifi_mqtt_client.h"
#include "Mqtt/mqtt_subscriber_manager.h"



#include "all_applications.h"
#ifdef I_AM_TEETH_WAREHOUSE_PACKER

Twh_Packer_Board board;

void test_board(){
    #define TESTING_BRIGHTNESS 11
    board.GetWs2812B()->TestLed(99, 1, 0, 0, TESTING_BRIGHTNESS);
    Serial.println("[Info] test_board() is done.");
}


void setup(){
    board.Init();
    // test_board();

    setup_wifi_mqtt_blocking_mode();

    board.GetButton_picked()->SetMqttPublishTopic("twh/221109/packer/button/pick");
    board.GetButton_Packed()->SetMqttPublishTopic("twh/221109/packer/button/pack");
    
    MqttSubscriberManager::GetInstance().AddSubscriber("twh/221109/packer/led", board.GetWs2812B());

    Logger::Info ("Twh Packer setup() is done. ");
    board.BootTest();
}


void loop(){
    board.GetButton_Packed()->SpinOnce();
    board.GetButton_picked()->SpinOnce();
}

#endif
