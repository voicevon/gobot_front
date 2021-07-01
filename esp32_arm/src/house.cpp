#include "house.h"

#define PIN_LEFT_SERVO 22
#define PIN_RIGHT_SERVO 23


House::House(){
  __Mcp23018 = &Mcp23018::getInstance();
  __Mcp23018->DisableAllCoils();
  __LeftServo.attach(PIN_LEFT_SERVO);
  __RightServo.attach(PIN_RIGHT_SERVO);

}

void House::Setup(RobotAction* pAction){
  __house_action = pAction;
  __current_coil_index = 0;
}


void House::SpinOnce(){
  switch (__house_action->bytes[0]){
    case 0:
      break;
    case 2:
      MoveStoneToTarget(3);
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

  // Bottom mover:  Leave original position
  __LeftServo.write(180);
  __RightServo.write(0);

    // Stop the coil
  __Mcp23018->EnableSingleCoil(house_id, false);


}


