#include "house.h"


House::House(){
  
}

void House::Init(){

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

  void House::EnableExpandPin(int phisical_pin_id){
    
  }

  void House::MoveStoneToTarget(int start_point){
      int logic_pin = start_point;
      
      int out_pin_id = start_point;
  }


void House::DrawStone(int house_id){
  // Bottom mover : move to original position
  this->__LeftServo.write(0);
  this->__RightServo.write(180);
  // Enable a coil to stick a stone

  // Bottom mover:  Leave original position

  // Stop the coil


}


