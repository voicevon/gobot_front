#include <Arduino.h>
#include "ble_server.h"


BleServer ble_server;


void setup(){
  //BLE setup
  Serial.begin(9600);
  ble_server.Init();
}


void home_x(){

}

void draw_stone(){

}

void loop(){
  ble_server.SpinOnce();
  int arm_action_code = ble_server.arm_action.action_code;
  switch (arm_action_code){
    case 0:
      break;
    case 2:
      break;
    case 8: //home_X
      ble_server.UpdateActionCode(8+1);
      home_x();
      ble_server.UpdateActionCode(0);
      break;
  }
  int house_action =1;
  switch (house_action){
    case 0:
      break;
    case 2:
      break;
    case 6:
      draw_stone();
      break;
  }

}