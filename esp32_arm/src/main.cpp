#include <Arduino.h>
#include "ble_server.h"
#include "arm.h"
#include "house.h" 

BleServer ble_server;
Arm arm;
House house;

void setup(){
  //BLE setup
  Serial.begin(9600);
  ble_server.Init();
  arm.Init();
  house.Init();
}
int i = 65;
void loop(){

  ble_server.SpinOnce();
  arm.SpinOnce(ble_server.arm_action);
  // house.SpinOnce(ble_server.house_action);
  ble_server.UpdateActionCode(i);
  delay(2000);
  i++;
}