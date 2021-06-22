#include<math.h>
#include "arm.h"


Arm::Arm(){
    // Configure each stepper
    this->stepper1.setMaxSpeed(100);
    this->stepper2.setMaxSpeed(100);

    // Then give them to MultiStepper to manage
    steppers.addStepper(this->stepper1);
    steppers.addStepper(this->stepper2);
}

void Arm::Init(){

}

void Arm::Home(unsigned char axis){
  unsigned int home_pin =23;
  AccelStepper* stepper;
  if (axis == 1 ){
    home_pin = this->alpha_home_pin;
    stepper = & this->stepper1;
  }
  else if (axis ==2){
    home_pin = this->beta_home_pin;
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

/*
https://github.com/ddelago/5-Bar-Parallel-Robot-Kinematics-Simulation/blob/master/fiveBar_InvKinematics.py
*/
motor_position Arm::ik(int x, int y){
    motor_position pos;

    // Angle from left shoulder to end effector
    double beta1 = atan2( y, (l0 + x) );

    // Angle from right shoulder to end effector
    double beta2 = atan2( y, (l0 - x) );

    // Alpha angle pre-calculations
    // alpha1_calc = (l1**2 + ( (l0 + x)**2 + y**2 ) - l2**2) / (2*l1*math.sqrt( (l0 + x)**2 + y**2 ))  
    // alpha2_calc = (l1**2 + ( (l0 - x)**2 + y**2 ) - l2**2) / (2*l1*math.sqrt( (l0 - x)**2 + y**2 )) 
    double alpha1_calc = (l1*l1 + ( (l0 + x)*(l0+x) + y*y ) - l2*l2) / (2*l1*sqrt( (l0 + x)*(l0+x) + y*y)); 
    double alpha2_calc = (l1*l1 + ( (l0 - x)*(l0-x) + y*y ) - l2*l2) / (2*l1*sqrt( (l0 - x)*(l0-x) + y*y));

    // If calculations > 1, will fail acos function
    if (alpha1_calc > 1 or alpha2_calc > 1){
        // print("Unreachable coordinates");
        pos.alpha = -1;
        pos.beta = -1 ;
        return pos;
    }

    // Angle of left shoulder - beta1 and right shoulder - beta2
    float alpha1 = acos(alpha1_calc);
    float alpha2 = acos(alpha2_calc);

    // Angles of left and right shoulders
    pos.alpha = beta1 + alpha1;
    pos.beta = 3.14159265 - beta2 - alpha2;
    
  return pos;
}


void Arm::pick_place_park(ArmAction arm_action){
  long positions[2];
  motor_position pos = ik(arm_action.Attr.pickup_x, arm_action.Attr.pickup_y);

  positions[0] = pos.alpha;
  positions[1] = pos.beta;

  steppers.moveTo(positions);
}


void Arm::SpinOnce(ArmAction action){
  switch (action.Attr.action_code){
    case 0:
      break;
    case 2:   // pickup and place and park
      __ble_server->UpdateActionCode(2+1);
      pick_place_park(action);
      break;
    case 8: //home_X
      // ble_server.UpdateActionCode(8+1);
      __ble_server->UpdateActionCode(8+1);
      this->Home(0);
      break;
    case 10:    //home y
      // ble_server.UpdateActionCode(10+1);
      Home(1);
      break;
    
  }
  __ble_server->UpdateActionCode(1);
}
