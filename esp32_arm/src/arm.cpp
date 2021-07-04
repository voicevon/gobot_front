#include<math.h>
#include "arm.h"
#include<Arduino.h>


#define PIN_EEF_SERVO 21
#define PIN_EEF_A  18
#define PIN_EEF_B  19

#define PIN_HOME_ALHPA 16
#define PIN_HOME_BETA 17

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


#define HOMED_POSITION_ALPHA 123
#define HOMED_POSITION_BETA 112

#define STEPS_PER_RAD 123
#define MOTOR_MAX_SPEED 100  /// unit?

Arm::Arm(){

}

void Arm::Home(unsigned char axis){  
  uint8_t homing_axis = axis;
  uint8_t home_pin = PIN_HOME_BETA;
  Stepper* homing_stepper = stepper_beta;

  if (axis == 4 ){
    home_pin = PIN_HOME_ALHPA;
    homing_stepper = stepper_alpha;
    // stepper = stepper_alpha;
    Serial.print("\nStart Homing Alpha");
  }
  else {
    // axis == 5
    // homing_stepper = stepper_beta;
    Serial.print("\nStart Homing Beta");
  }
  homing_stepper->setTargetRel(-500);
  steppers->move(*homing_stepper);
  while (steppers->isRunning()){
    //Seems alway return false of isRunning()
    Serial.print("r");
  };

  __HomeSpin(homing_stepper, home_pin);
  if (homing_axis == 4){
    stepper_alpha->setPosition(HOMED_POSITION_ALPHA);
  }
  else{
    stepper_beta->setPosition(HOMED_POSITION_BETA);
  }
  __arm_action->bytes[0] = 0;
  Serial.print("\n Home one axis is Done.......");
}

void Arm::__HomeSpin(Stepper* homing_stepper, uint8_t home_pin ){
  uint8_t flags=0;
  // Serial.print("\nHome spin got started.............\n");
  while (flags !=0xff){
    stepper_alpha->setTargetRel(1);
    steppers->move(*homing_stepper);
    flags <<= 1;
    flags |= digitalRead(home_pin);
  }
  // Serial.print("\n###################  Home is done.");
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
  if (pos.alpha ==-1){
    Serial.println("IK return -1");
    return;
  }
  stepper_alpha->setTargetAbs(pos.alpha);
  stepper_beta->setTargetAbs(pos.beta);
  steppers->move(*stepper_alpha, * stepper_beta);
}

void Arm::SetEffector(EEF action){
  switch (action){
    case Lower:
      eefServo->write(180);
      break;
    case Higher:
      eefServo->write(0);
      break;
    case Suck:
      digitalWrite(PIN_EEF_A, HIGH);
      digitalWrite(PIN_EEF_B, LOW);
      break;
    case Release:
      digitalWrite(PIN_EEF_A,LOW);
      digitalWrite(PIN_EEF_B,HIGH);
      break;
    case Sleep:
      digitalWrite(PIN_EEF_A,LOW);
      digitalWrite(PIN_EEF_B,LOW);
      break;
    default:
      break;

  }
}

void Arm::pick_place_park(RobotAction* pAction){
  uint8_t action_code = pAction->Arm.action_code;
  if ((action_code & (1<<1)) >= 1){
    MoveTo(pAction->Arm.pickup_x, pAction->Arm.pickup_y);
    SetEffector(Lower);
    SetEffector(Suck);
    SetEffector(Higher);
  }
  if ((action_code & (1<<2)) >= 1){
    MoveTo(pAction->Arm.place_x, pAction->Arm.place_y);
    SetEffector(Lower);
    SetEffector(Release);
    SetEffector(Higher);
    SetEffector(Sleep);
  }
  if ((action_code & (1<<3)) >= 1){
    MoveTo(pAction->Arm.park_x, pAction->Arm.park_y);
    SetEffector(Sleep);
  }
}

void Arm::Setup(RobotAction* pAction){
  __arm_action = pAction;
  // Serial.print("Arm is Initializing.........");
  // __Mcp23018 = &Mcp23018::getInstance();
  Servo sv = Servo();
  sv.attach(PIN_EEF_SERVO);
  eefServo = &sv ;


  pinMode(PIN_EEF_A, OUTPUT);
  pinMode(PIN_EEF_B,OUTPUT);
  SetEffector(Sleep);
  pinMode(PIN_BETA_ENABLE, OUTPUT);
  digitalWrite(PIN_BETA_ENABLE, HIGH);

  pinMode(PIN_HOME_ALHPA,INPUT_PULLUP);
  pinMode(PIN_HOME_BETA, INPUT_PULLUP);

  // With libery AccelStepper
  // AccelStepper stepper = AccelStepper(AccelStepper::MotorInterfaceType::DRIVER, 
  //                                     PIN_ALPHA_STEP, PIN_ALPHA_DIR);
  // stepper_alpha = & stepper;
  // steppers.addStepper(stepper);
  // stepper = AccelStepper(AccelStepper::MotorInterfaceType::DRIVER, 
  //                       PIN_BETA_STEP,PIN_BETA_DIR);
  // stepper_beta = & stepper;
  // steppers.addStepper(stepper);


  // With liberary ESP32Step
  stepper_alpha = new Stepper(PIN_ALPHA_STEP, PIN_ALPHA_DIR);
  // stepper_alpha = Stepper(PIN_ALPHA_STEP, PIN_ALPHA_DIR);
  stepper_beta = new Stepper(PIN_BETA_STEP, PIN_BETA_DIR);

  stepper_alpha->setMaxSpeed(MOTOR_MAX_SPEED);
  stepper_beta->setMaxSpeed(MOTOR_MAX_SPEED);
  digitalWrite(PIN_BETA_ENABLE, LOW);
  // Serial.print("\n@@@@@@@@@@@@@@@@@@");
  steppers = new StepControl();
  // Serial.print("@@@@@@@@@@@@@@@@@@");
  // steppers = &controller;
  // link length in mm
  l0 = LINK_0;
  l1 = LINK_1;
  l2 = LINK_2;
}

void Arm::SpinOnce(){
  // steppers.run();
  return;
  // if (!homed)
  //   HomeSpin();
  // else if (!steppers.run())
  //   __arm_action->bytes[0] = 1;
}
