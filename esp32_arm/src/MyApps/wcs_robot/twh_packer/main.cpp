
#include "MyLibs/MyFunctions.hpp"
#include "board/board.h"
#include "Mqtt/wifi_mqtt_client.h"
#include "Mqtt/mqtt_subscriber_manager.h"



#include "all_applications.h"
#ifdef I_AM_TEETH_WAREHOUSE_PACKER

Twh_Packer_Board board;

void test_board(){
    #define BRIGHTNESS 11
    // board.TestLed(0, 1, 0, 0, 0);
    // board.TestLed(0, 2, 0, BRIGHTNESS, 0);
    // board.TestLed(0, 3, 0, 0, BRIGHTNESS);
    board.TestButton(0);
    Serial.println("[Info] test_board() is done.");
}


void setup(){
    board.Init();
    test_board();

    setup_wifi_mqtt();

    board.GetButton_picked()->SetMqttPublishTopic("twh/221109/packer/button/pick");
    board.GetButton_Packed()->SetMqttPublishTopic("twh/221109/packer/button/pack");
    
    MqttSubscriberManager::GetInstance().AddSubscriber("twh/221109/packer/led", board.GetLeds());
    Logger::Info ("Teeth Warehouse   setup() is done. ");
}


void loop(){
    board.GetButton_Packed()->SpinOnce();
    board.GetButton_picked()->SpinOnce();
}

#endif
