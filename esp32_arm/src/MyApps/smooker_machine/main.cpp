//https://www.electronicshub.org/wp-content/uploads/2021/02/ESP32-Pinout-1.jpg

#include "all_devices.h"
#ifdef I_AM_SMOKE_MACHINE

#include "Iot/wifi_mqtt_client.h"
#include "MyLibs/MyFunctions.hpp"

#include <HardwareSerial.h>
#include "smoke_bot.h"
#include "Robot/Gcode.h"
// #include <Arduino.h>

std::string topic = "actu/dev001/yongquan"; 
std::string payload ="OFF";

SmokeBot mybot;
int distance = 100;
int pause_second = 20;
Gcode gcode("");
std::string ss;

void app_mqtt_subscribe(){
  mqttClient.subscribe("smokebot/distance", 2);
  mqttClient.subscribe("smokebot/pause_second", 2);
  Serial.println("Subscribed smokebot/*");
}

void setup() {
  Serial.begin(115200);
  Serial.println("Hi there, I am smoke robot, Have a good day");
  setup_wifi_mqtt();
  // mybot.HomeAllAxises();
  mybot.Init_Linkage();
  Serial.print("Set up is done .....");
}



void loop() {

  ss="G1";
  ss.append(ToString(distance));
  // Serial.println("  111111111111111 ");
  gcode = Gcode(ss);
  // Serial.print (ss.c_str());
  // Serial.println("  2222222222222 ");
  delay(100);
  mybot.RunGcode(&gcode);
  
  ss = "G4";
  ss.append(ToString(pause_second));
  gcode = Gcode(ss);
  mybot.RunGcode(&gcode);

  ss = "G1X0";
  gcode = Gcode(ss);
  mybot.RunGcode(&gcode);

}
#endif