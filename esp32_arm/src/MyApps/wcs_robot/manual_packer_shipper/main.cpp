
#include "MyLibs/MyFunctions.hpp"
#include "board/board.h"
#include "Mqtt/wifi_mqtt_client.h"
#include "Mqtt/mqtt_subscriber_manager.h"
#include "Mqtt/remote_binary_output_group.h"


#include "all_applications.h"
#ifdef I_AM_WAREHOUSE_MANUAL_PACKER_SHIPPER
#define LEDS_COUNT 12

Twh_Packer_Board board;
RemoteBinaryOutputGroup leds_placing;
RemoteBinaryOutputGroup leds_shipping;

void test_board(){
    #define TESTING_BRIGHTNESS 11
    board.GetWs2812B()->TestLed(99, 1, 0, 0, TESTING_BRIGHTNESS);
    Serial.println("[Info] test_board() is done.");
}



void setup(){
    board.Init();
    // test_board();

    setup_wifi_mqtt_blocking_mode();

    board.GetButton_picked()->SetMqttPublishTopic("wh221109/packer/button/pick");
    board.GetButton_Packed()->SetMqttPublishTopic("wh221109/packer/button/pack");
    leds_placing.Init(12);
    leds_shipping.Init(12);
    gs_MqttSubscriberManager::Instance().AddSubscriber("wh221109/placing_leds", &leds_placing);
    gs_MqttSubscriberManager::Instance().AddSubscriber("wh221109/shipping_leds",&leds_shipping);

    Logger::Info ("Twh Packer setup() is done. ");
    board.BootTest();
}

void show_leds(){
    // char* leds_packing_command;  
    // char* leds_shipping_command; 
    board.GetWs2812B()->Clear();

    char* leds_packing_command = leds_placing.Get();  // 'N' => ON, 'F' => OFF
    for(int i=0; i<LEDS_COUNT; i++){
        if (*(leds_packing_command+i) == 'N')
            board.GetWs2812B()->SetLed_Green(i);
    }
    char* leds_shipping_command = leds_placing.Get();  // 'N' => ON, 'F' => OFF
    for(int i=0; i<LEDS_COUNT; i++){
        if (*(leds_shipping_command+i) == 'N')
            board.GetWs2812B()->SetLed_Blue(i);
    }

}


void loop(){
    board.GetButton_Packed()->SpinOnce();
    board.GetButton_picked()->SpinOnce();
    show_leds();
}

#endif
