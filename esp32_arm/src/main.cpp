#include <Arduino.h>
#include "ble_server.h"
#include "arm.h"
#include "house.h" 
#include "test.h"



BleServer* ble_server = &BleServer::getInstance();
Arm* arm = &Arm::getInstance();
House* house = &House::getInstance();
Runner runner();

void setup(){
  //BLE setup
  Serial.begin(9600);
  ble_server->Init();
}
int i = 65;
void loop(){

  ble_server->SpinOnce();
  arm->SpinOnce(&ble_server->body_action);
  // house.SpinOnce(ble_server.house_action);
  ble_server->UpdateActionCode(i);
  delay(2000);
  i++;
}