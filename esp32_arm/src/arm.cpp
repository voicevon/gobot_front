#include<math.h>
#include "arm.h"
#include<Arduino.h>


#define PIN_EEF_SERVO 12
#define PIN_HOME_ALHPA 13
#define PIN_HOME_BETA 14

#define STEPS_PER_RAD 123
#define MOTOR_MAX_SPEED 100  /// unit?

#define PIN_ALPHA_STEP 15
#define PIN_ALPHA_DIR 16
#define PIN_ALPHA_ENABLE 17
#define PIN_BETA_STEP 18
#define PIN_BETA_DIR 19
#define PIN_BETA_ENABLE 17
//  unit is mm
#define LINK_0 22.3  // Length between origin and the two motors
#define LINK_1 144.4 // Length from motor to passive joints
#define LINK_2 255.5 // Length from passive joints to end effector

Arm::Arm(){

  // __Mcp23018 = &Mcp23018::getInstance();

  AccelStepper stepper = AccelStepper(AccelStepper::MotorInterfaceType::FULL4WIRE, 5,6,7,8,false);
  stepper_alpha = & stepper;
  stepper = AccelStepper(AccelStepper::MotorInterfaceType::FULL4WIRE, 2,3,4,5,false);
  stepper_beta = & stepper;
  stepper_alpha->setMaxSpeed(MOTOR_MAX_SPEED);
  stepper_beta->setMaxSpeed(MOTOR_MAX_SPEED);

  // Then give them to MultiStepper to manage
  steppers->addStepper(*stepper_alpha);
  steppers->addStepper(*stepper_beta);

  Servo sv = Servo();
  sv.attach(PIN_EEF_SERVO);
  eefServo = &sv ;

  // link length in mm
  l0 = LINK_0;
  l1 = LINK_1;
  l2 = LINK_2;
}

void Arm::Home(unsigned char axis){
  unsigned int home_pin =23;
  AccelStepper* stepper;
  if (axis == 1 ){
    home_pin = PIN_HOME_ALHPA;
    stepper = stepper_alpha;
  }
  else if (axis ==2){
    home_pin = PIN_HOME_BETA;
    stepper = stepper_beta;
  }

  bool homed = false;
  do
  {
    stepper->setCurrentPosition(0);
    stepper->move(1);
    homed = digitalRead(home_pin);
  } while (!homed);
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
    pos.alpha = (beta1 + alpha1) * STEPS_PER_RAD;
    pos.beta = (3.14159265 - beta2 - alpha2) * STEPS_PER_RAD;
    
  return pos;
}

void Arm::MoveTo(int x, int y){
  motor_position pos=ik(x,y);
  long angles[2];
  angles[0] = pos.alpha;
  angles[1] = pos.beta;
  steppers->moveTo(angles);
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

void Arm::pick_place_park(BodyAction* body_action){
  uint8_t action_code = body_action->Arm.action_code;
  if (action_code & 1<<2 == 1){
    MoveTo(body_action->Arm.pickup_x, body_action->Arm.pickup_y);
    SetEffector(Lower);
    SetEffector(Suck);
    SetEffector(Higher);
  }
  if (action_code & 1<<3 == 1){
    MoveTo(body_action->Arm.place_x, body_action->Arm.place_y);
    SetEffector(Lower);
    SetEffector(Release);
    SetEffector(Higher);
    SetEffector(Sleep);
  }
  if (action_code & 1<<4 == 1){
    MoveTo(body_action->Arm.park_x, body_action->Arm.park_y);
    SetEffector(Sleep);
  }
}

void Arm::SpinOnce(BodyAction* action){
  // return;
  int ccc = __ble_server->body_action.Arm.action_code;
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
