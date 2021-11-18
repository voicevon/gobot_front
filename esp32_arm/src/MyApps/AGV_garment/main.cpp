#include "all_devices.h"
#ifdef I_AM_GARMENT_BOT

#include "Iot/wifi_mqtt_client.h"

#include "MyLibs/MyFunctions.hpp"
#include "garment_bot.h"
#include "Robot/Gcode.h" 

#define PIN_ENCODER_A 12
#define PIN_ENCODER_B 13

GarmentBot mybot = GarmentBot();
IrEncoder irEncoder_leftWheel = IrEncoder(PIN_ENCODER_A, PIN_ENCODER_B, 40);
IrEncoderHelper irEncoderHelper = IrEncoderHelper();
void doB(){irEncoder_leftWheel.handleB();}

void app_mqtt_subscribe(){
  mqttClient.subscribe("garmentbot/mode", 3);
  Serial.println("Subscribed garmentbot/*");
}

void app_mqtt_received_message( char* topic, char* payload){
  const char * cc = (const char*)(payload);

  if(strcmp(topic, "garmentbot/mode") == 0) {   // char len = 17
    GARMENTBOT_MODE mode = (GARMENTBOT_MODE)(atoi(cc));
    mybot.SetMode(mode);
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
    Serial.println("Hi there, I am your lovely bot,  CableBot-Corner.  Keep smiling :)");
    // irEncoder_leftWheel.pullup= Pullup::USE_EXTERN;
    // irEncoder_leftWheel.init();
    // irEncoder_leftWheel.enableInterrupts(doB);
    // irEncoderHelper.LinkSensor(&irEncoder_leftWheel);
    // irEncoderHelper.InitFormula_LinearEquation(3.613f, 0.0f);
    
    mybot.Init();
    // mybot.Init_Linkage(&irEncoderHelper);
    setup_wifi_mqtt();
    Serial.println ("\n\nSetup is done. ------------------------------------ ");

    while (!mqttClient.connected())
        delay(100);

}

bool tested = false;
void loop(){
    // bool x = digitalRead(32);
    // Serial.print(x);
    // delay(1000);
    // mybot.SpinOnce();
    
    if (!tested){
        // robot.test_home();
        // tested = true;
        // Gcode gcode = Gcode("M119");
        // robot.RunGcode(&gcode);
        // delay(100);
    }
}

#endif