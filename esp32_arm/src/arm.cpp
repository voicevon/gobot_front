#include<math.h>
#include "arm.h"
#include<Arduino.h>

Arm::Arm(){
    // Configure each stepper
    this->stepper_alpha.setMaxSpeed(100);
    this->stepper_beta.setMaxSpeed(100);

    // Then give them to MultiStepper to manage
    steppers.addStepper(this->stepper_alpha);
    steppers.addStepper(this->stepper_beta);
}

void Arm::Init(){

}

void Arm::Home(unsigned char axis){
  unsigned int home_pin =23;
  AccelStepper* stepper;
  if (axis == 1 ){
    home_pin = this->alpha_home_pin;
    stepper = & this->stepper_alpha;
  }
  else if (axis ==2){
    home_pin = this->beta_home_pin;
    stepper = & this->stepper_beta;
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

void Arm::MoveTo(int x, int y){
  motor_position pos=ik(x,y);
  long angles[2];
  angles[0] = pos.alpha;
  angles[1] = pos.beta;
  steppers.moveTo(angles);
}

void Arm::SetEffector(EEF action){
  switch (action){
    case Lower:
      break;
    case Higher:
      break;
    case Suck:
      break;
    case Release:
      break;
    case Sleep:
      break;
    default:
      break;

  }
}

void Arm::pick_place_park(ArmAction* arm_action){
  long positions[2];
  if (arm_action->Attr.pickup_type ==1){
    MoveTo(arm_action->Attr.pickup_x, arm_action->Attr.pickup_y);
    SetEffector(Lower);
    SetEffector(Suck);
    SetEffector(Higher);
  }
  if (arm_action->Attr.place_type == 1){
    MoveTo(arm_action->Attr.place_x, arm_action->Attr.place_y);
    SetEffector(Lower);
    SetEffector(Release);
    SetEffector(Higher);
    SetEffector(Sleep);
  }
  if (arm_action->Attr.do_park == 1){
    MoveTo(arm_action->Attr.park_x, arm_action->Attr.park_y);
    SetEffector(Sleep);
  }
}

void Arm::SpinOnce(ArmAction action){
  // return;
  int ccc = __ble_server->arm_action.Attr.action_code;
  // return;
  Serial.println("The new aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa value is: ");
  // Serial.println(ccc);

  return;



  int code = 0;
  switch (code){
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
    default:
      break;
    
  }
  __ble_server->UpdateActionCode(1);
}
