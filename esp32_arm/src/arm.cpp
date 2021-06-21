#include "arm.h"

// Arm::Arm(){
//     // // Configure each stepper
//     // this->stepper1.setMaxSpeed(100);
//     // this.stepper2.setMaxSpeed(100);

//     // // Then give them to MultiStepper to manage
//     // steppers.addStepper(this.stepper1);
//     // steppers.addStepper(this.stepper2);
// }

void Arm::Init(){

}

void Arm::Home(unsigned char axis){
  unsigned char home_pin =23;
  AccelStepper* stepper;
  if (axis == 1 ){
    home_pin = 24;
    stepper = & this->stepper1;
  }
  else if (axis ==2){
    home_pin = 24;
    stepper = & this->stepper2;
  }
  bool homed = false;
  do
  {
    stepper->setCurrentPosition(0);
    stepper->move(100);
    homed = digitalRead(home_pin);
  } while (homed);
}

void Arm::pick_place_park(ArmAction arm_action){
  long positions[2];
  positions[0] = arm_action.pickup_x;
  positions[1] = arm_action.pickup_y;

  steppers.moveTo(positions);
}


void Arm::SpinOnce(ArmAction action){
  switch (action.action_code){
    case 0:
      break;
    case 2:   // pickup and place and park
      // ble_server.UpdateActionCode(2+1);
      pick_place_park(action);
      // ble_server.UpdateActionCode(0);      
      break;
    case 8: //home_X
      // ble_server.UpdateActionCode(8+1);
      this->Home(0);
      // ble_server.UpdateActionCode(0);
      break;
    case 10:    //home y
      // ble_server.UpdateActionCode(10+1);
      Home(1);
      // ble_server.UpdateActionCode(0);
      break;
      
  }
}
