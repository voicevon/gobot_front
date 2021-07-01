#include<math.h>
#include "arm.h"
#include<Arduino.h>


#define PIN_EEF_SERVO 21
#define PIN_EEF_A  18
#define PIN_EEF_B  19
#define PIN_HOME_ALHPA 16
#define PIN_HOME_BETA 17

#define PIN_MOVER_LEFT_SERVO 4
#define PIN_MOVER_RIGHT_SERVO 5


#define STEPS_PER_RAD 123
#define MOTOR_MAX_SPEED 100  /// unit?

#define PIN_ALPHA_STEP 12
#define PIN_ALPHA_DIR 26
#define PIN_ALPHA_ENABLE 13
#define PIN_BETA_STEP 14
#define PIN_BETA_DIR 25
#define PIN_BETA_ENABLE 13


//  unit is mm
#define LINK_0 191.0  // Length between origin and the two motors
#define LINK_1 285.18 // Length from motor to passive joints
#define LINK_2 384.51 // Length from passive joints to end effector

// RobotAction* arm_action;
Arm::Arm(){
  __Mcp23018 = &Mcp23018::getInstance();
  Servo sv = Servo();
  sv.attach(PIN_EEF_SERVO);
  eefServo = &sv ;
  
  sv=Servo();
  sv.attach(PIN_MOVER_LEFT_SERVO);
  mover_left_Servo = &sv;

  sv= Servo();
  sv.attach(PIN_MOVER_RIGHT_SERVO);
  mover_right_Servo = &sv;

  pinMode(PIN_EEF_A, OUTPUT);
  digitalWrite(PIN_EEF_A, HIGH);
  pinMode(PIN_EEF_B,OUTPUT);
  digitalWrite(PIN_EEF_B, HIGH);
  pinMode(PIN_BETA_ENABLE, OUTPUT);
  digitalWrite(PIN_BETA_ENABLE, HIGH);

  pinMode(PIN_HOME_ALHPA,INPUT_PULLUP);
  pinMode(PIN_HOME_BETA, INPUT_PULLUP);

  AccelStepper stepper = AccelStepper(AccelStepper::MotorInterfaceType::DRIVER, 
                                      PIN_ALPHA_STEP, PIN_ALPHA_DIR);
  stepper_alpha = & stepper;
  steppers.addStepper(stepper);

  stepper = AccelStepper(AccelStepper::MotorInterfaceType::DRIVER, 
                        PIN_BETA_STEP,PIN_BETA_DIR);
  stepper_beta = & stepper;
  steppers.addStepper(stepper);

  stepper_alpha->setMaxSpeed(MOTOR_MAX_SPEED);
  stepper_beta->setMaxSpeed(MOTOR_MAX_SPEED);

  // These code cause the ESP32-Core being cracked ??
  // steppers->addStepper(*stepper_alpha);
  // steppers->addStepper(*stepper_beta);


#define PIN_EEF_B  19
  // link length in mm
  l0 = LINK_0;
  l1 = LINK_1;
  l2 = LINK_2;
}

void Arm::Home(unsigned char axis){
  uint8_t home_pin = 0;
  AccelStepper* stepper;
  
  if (axis == 4 ){
    home_pin = PIN_HOME_ALHPA;
    stepper = stepper_alpha;
  }
  else {
    // axis == 5
    home_pin = PIN_HOME_BETA;
    stepper = stepper_beta;
  }

  bool homed = false;
  stepper->setCurrentPosition(0);
  stepper->move(100);
  homed = digitalRead(home_pin);
  if (homed){
    __arm_action->bytes[0] = 0;
  }else{
    __arm_action->bytes[0] = (1<< axis) + 1;
  }
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

void Arm::pick_place_park(RobotAction* pAction){
  uint8_t action_code = pAction->Arm.action_code;
  if (action_code & 1<<2 == 1){
    MoveTo(pAction->Arm.pickup_x, pAction->Arm.pickup_y);
    SetEffector(Lower);
    SetEffector(Suck);
    SetEffector(Higher);
  }
  if (action_code & 1<<3 == 1){
    MoveTo(pAction->Arm.place_x, pAction->Arm.place_y);
    SetEffector(Lower);
    SetEffector(Release);
    SetEffector(Higher);
    SetEffector(Sleep);
  }
  if (action_code & 1<<4 == 1){
    MoveTo(pAction->Arm.park_x, pAction->Arm.park_y);
    SetEffector(Sleep);
  }
}

void Arm::Setup(RobotAction* pAction){
  __arm_action = pAction;
}

void Arm::SpinOnce(){
  if (steppers.run()) 
    return;

  int code = __arm_action->bytes[0] & 0b11111110;
  switch (code){
    case 0:
      __arm_action->bytes[0] = 1;
      break;

    case 2:   // pickup and place and park
      // __ble_server->SetActionCode(2+1);
      pick_place_park(__arm_action);
      break;
    case 1<<4: //home_X
      Home(4);
      break;
    case 1<<5:
      Home(5);
      break;
    default:
      break;
    
  }
}
