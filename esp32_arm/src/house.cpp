#include "house.h"

#define PIN_LEFT_SERVO 22
#define PIN_RIGHT_SERVO 23



House::House(){
  __Mcp23018 = &Mcp23018::getInstance();
  __LeftServo.attach(PIN_LEFT_SERVO);
  __RightServo.attach(PIN_RIGHT_SERVO);

}


void House::SpinOnce(HouseAction action){
  switch (action.action_code){
    case 0:
      break;
    case 2:
      break;
    case 6:
      DrawStone(3);
      break;
  }
}



  void House::MoveStoneToTarget(int start_point){
    int logic_pin = start_point;
    
    int out_pin_id = start_point;
  }


void House::DrawStone(int house_id){
  // Bottom mover : move to original position
  __LeftServo.write(0);
  __RightServo.write(180);
  // Enable a coil to stick a stone
  __Mcp23018->EnableSingleCoil(house_id, true);
  // __EnableSingleCoil(house_id, true);
  // Bottom mover:  Leave original position
  __LeftServo.write(180);
  __RightServo.write(0);
    // Stop the coil
  __Mcp23018->EnableSingleCoil(house_id, false);
  // __EnableSingleCoil(house_id, false);

}


