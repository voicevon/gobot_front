#include "actions.h"

void test_action_size_2021(){
  RobotAction action;
  action.Arm.action_code = 0;
  action.Arm.pickup_x= -1;
  action.Arm.pickup_y = 2;
  action.Arm.place_x = -3;
  action.Arm.place_y = 4;
  action.Arm.park_x = -5;
  action.Arm.park_y = 6;

  Serial.print(sizeof(action)) ;
  Serial.print("?????????        ");
  for (int i =0 ; i<13; i++){
    Serial.print(action.bytes[i]);
    Serial.print("  ");
  }
  while (true)
    {

    }
}