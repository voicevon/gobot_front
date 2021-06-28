#include <Arduino.h>
#include "ble_server.h"
#include "arm.h"
#include "house.h" 
#include "test.h"



BleServer* ble_server = &BleServer::getInstance();
Arm* arm = &Arm::getInstance();
// House* house = &House::getInstance();

void setup(){
  Serial.begin(115200);
  ble_server->Init();
}
int i = 65;
void loop(){
  Serial.print(i);
  Serial.print("   ");
  ble_server->SpinOnce();
  // arm->SpinOnce(&ble_server->body_action);
  // ble_server->UpdateActionCode(i);
  delay(2000);
  i++;
}