//https://www.electronicshub.org/wp-content/uploads/2021/02/ESP32-Pinout-1.jpg

#include "all_devices.h"
#ifdef I_AM_SMOKE_MACHINE

#include "Iot/wifi_mqtt_client.h"
#include "MyLibs/MyFunctions.hpp"

#include <HardwareSerial.h>
#include "smoke_bot.h"
#include "Robot/Gcode.h"
// #include <Arduino.h>

std::string topic = "actu/foot/yongquanxue"; 
std::string payload ="123456";

SmokeBot mybot;


void setup() {
  Serial.begin(115200);
  setup_wifi_mqtt();
  mybot.HomeAllAxises();
}

int distance = 100;
int pause_second = 20;
Gcode gcode("");
std::string ss;

void loop() {
  ss="G1";
  ss.append(ToString(distance));
  gcode = Gcode(ss);
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