//https://www.electronicshub.org/wp-content/uploads/2021/02/ESP32-Pinout-1.jpg

#include "all_devices.h"
#ifdef I_AM_SMOKE_MACHINE

// #include "Iot/wifi_mqtt_client.h"
#include "MyLibs/MyFunctions.hpp"

#include <HardwareSerial.h>
#include "smoke_bot.h"
#include "Robot/Gcode.h"
// #include <Arduino.h>
#include "webCommu.h"

// std::string topic = "actu/dev001/yongquan"; 
// std::string payload ="OFF";

SmokeBot* mybot;
// int distance = 100;
// int pause_second = 20;
Gcode gcode("");
std::string strGcode;

// void app_mqtt_subscribe(){
//   mqttClient.subscribe("smokebot/distance", 2);
//   mqttClient.subscribe("smokebot/pause_second", 2);
//   Serial.println("Subscribed smokebot/*");
// }

// void app_mqtt_received_message( char* topic, char* payload){
//   const char * cc = (const char*)(payload);

//   if(strcmp(topic, "smokebot/distance") == 0) {   // char len = 17
//     distance = atof(cc);
//   }
//   else if(strcmp(topic, "smokebot/pause_second")==0){    // char len = 21
//     pause_second = atof(cc);
//   }else{
//     Serial.print(" app_mqtt_received_message() Warning   ");
//     Serial.print (topic);
//     Serial.print("   ");
//     Serial.print (payload);
//   }

// }

void setup() {
  Serial.begin(115200);
  Serial.println("Hi there, I am smoke robot, Have a good day");
  setup_webcommu();
  // setup_wifi_mqtt();
  mybot = new SmokeBot();
  mybot->Init_Gpio();
  mybot->Init_Linkage();
  Serial.print("Set up is done .....");
  // while (!mqttClient.connected())
  //     delay(100);
  // mybot->HomeAllAxises();
}

void loop() {
  loop_webcommu();
  float distance = float(var_volume) / 200.0;


  strGcode="G1 X";
  strGcode.append(ToString(distance));
  gcode = Gcode(strGcode);
  // Serial.print (ss.c_str());
  // delay(100);
  mybot->RunGcode(&gcode);
  strGcode = "G4 ";
  strGcode.append(ToString(var_sleep_time));
  gcode = Gcode(strGcode);
  mybot->RunGcode(&gcode);
  strGcode = "G1 X0";
  gcode = Gcode(strGcode); 

}
#endif