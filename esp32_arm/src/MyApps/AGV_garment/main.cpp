#include "all_devices.h"
#ifdef I_AM_GARMENT_BOT

#include "Iot/wifi_mqtt_client.h"

#include "MyLibs/MyFunctions.hpp"
#include "garment_bot.h"
#include <WiFi.h>

// #include "Robot/Commu/commu_esp_now.h"



#define PIN_ENCODER_A 12
#define PIN_ENCODER_B 13

GarmentBot *mybot; // = GarmentBot();
// CommuEspNow_Master commu_esp_now;
// IrEncoder irEncoder_leftWheel = IrEncoder(PIN_ENCODER_A, PIN_ENCODER_B, 40);
// IrEncoderHelper irEncoderHelper = IrEncoderHelper();
// void doB(){irEncoder_leftWheel.handleB();}

void app_mqtt_subscribe(){
    mqttClient.subscribe("garmentbot/mode", 2);
    Serial.println("Subscribed garmentbot/*");
}

void app_mqtt_received_message( char* topic, char* payload){
    const char * cc = (const char*)(payload);

    if(strcmp(topic, "garmentbot/mode") == 0) {   // char len = 17
        GarmentBot::GARMENTBOT_STATE mode = (GarmentBot::GARMENTBOT_STATE)(atoi(cc));
        mybot->ToState(mode);
    }
    else if(strcmp(topic, "smokebot/pause_second")==0){    // char len = 21

    }else{
        Serial.print(" app_mqtt_received_message() Warning   ");
        Serial.print (topic);
        Serial.print("   ");
        Serial.print (payload);
    }

}


void setup(){
    Serial.begin(115200);
    Serial.println("Hi there, I am your lovely bot,  Garmentbot AGV + BoxMover.  Keep smiling :)");
    // const uint8_t slave_mac_addr[] = {0x00,0x00,0x00,0x00,0x00,0x00};
    // commu_esp_now.InitMaster(&slave_mac_addr[0],1);
    mybot = new GarmentBot();
    mybot->Init();
    Serial.println ("\n\nSetup mybot is done. ------------------------------------ ");
    // setup_wifi_mqtt();
    Serial.println ("\n\nSetup_wifi_mqtt is done. ------------------------------------ ");

    // while (!mqttClient.connected())
    //     delay(100);
    
    mybot->Test(1);

}


void loop(){
	// Will execute top level commands , use cases are:
	//      Pickup from station:  123, and Drop box to station:  234
	//      Move to Charge station

    // mybot.Test(1);
    // mybot.Test(2);
    mybot->SpinOnce();


}

#endif