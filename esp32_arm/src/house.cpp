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
      DrawStone();
      break;
  }
}

void House::DrawStone(){

}


