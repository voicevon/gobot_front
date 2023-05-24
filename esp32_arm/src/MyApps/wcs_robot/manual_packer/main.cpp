
#include "MyLibs/MyFunctions.hpp"
#include "board/board.h"
#include "Mqtt/wifi_mqtt_client.h"
#include "Mqtt/mqtt_subscriber_manager.h"
#include "Mqtt/remote_binary_output_group.h"


#include "all_applications.h"
#ifdef I_AM_TEETH_WAREHOUSE_PACKER

Twh_Packer_Board board;


void test_board(){
    #define TESTING_BRIGHTNESS 11
    board.GetWs2812B()->TestLed(99, 1, 0, 0, TESTING_BRIGHTNESS);
    Serial.println("[Info] test_board() is done.");
}

RemoteLedsState leds_placing(12);
RemoteLedsState leds_shipping(12);

void setup(){
    board.Init();
    // test_board();

    setup_wifi_mqtt_blocking_mode();

    board.GetButton_picked()->SetMqttPublishTopic("wh221109/packer/button/pick");
    board.GetButton_Packed()->SetMqttPublishTopic("wh221109/packer/button/pack");
    
    gs_MqttSubscriberManager::Instance().AddSubscriber("wh221109/placing_leds", board.GetWs2812B());
    gs_MqttSubscriberManager::Instance().AddSubscriber("wh221109/shipping_leds", board.GetWs2812B());

    Logger::Info ("Twh Packer setup() is done. ");
    board.BootTest();
}

void show_leds(){
    char* leds_command;  // 'N' => ON, 'F' => OFF
    leds_command = led.Get();
    for(int i=0; i<LEDS_COUNT; i++){
        if (*(leds_command+i) == 'N')
            board.GetLed(i)->TurnOn();
        else
            board.GetLed(i)->TurnOff();
    }
}

void loop(){
    board.GetButton_Packed()->SpinOnce();
    board.GetButton_picked()->SpinOnce();
    show_leds();
}

#endif
