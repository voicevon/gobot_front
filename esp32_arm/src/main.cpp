#include <Arduino.h>
#include "ble_server.h"
#include "accel_stepper/MultiStepper.h"

BleServer ble_server;
MultiStepper steppers;


void setup(){
  //BLE setup
  Serial.begin(9600);
  ble_server.Init();
}


void home(unsigned char axis){
  unsigned char home_pin =23;
  if (axis == 1 ) home_pin = 24;
  bool homed = false;
  do
  {
    steppers[axis].SetCurrentPosition(0);
    steppers[axis].Move(100);
    homed = digitalRead(home_pin)
  } while (homed);
}

void pick_place_park(ArmAction arm_action){
  long positions[2];
  positions[0] = arm_action.pickup_x;
  positions[1] = arm_action.pickup_y;

  steppers.moveTo(positions)
}


void draw_stone(){

}

void arm_task(ArmAction action){
  switch (action.action_code){
    case 0:
      break;
    case 2:   // pickup and place and park
      ble_server.UpdateActionCode(2+1);
      pick_place_park(ble_server.arm_action);
      ble_server.UpdateActionCode(0);      
      break;
    case 8: //home_X
      ble_server.UpdateActionCode(8+1);
      home(0);
      ble_server.UpdateActionCode(0);
      break;
    case 10:    //home y
      ble_server.UpdateActionCode(10+1);
      home(1);
      ble_server.UpdateActionCode(0);
      break;
      
  }
}

void house_task(HouseAction action){
  switch (action.code){
    case 0:
      break;
    case 2:
      break;
    case 6:
      draw_stone();
      break;
  }
}


void loop(){
  ble_server.SpinOnce();
  arm_task(ble_server.arm_action);
  house_task(ble_server.house_action);
}