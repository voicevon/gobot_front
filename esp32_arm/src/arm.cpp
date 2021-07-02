#include<math.h>
#include "arm.h"
#include<Arduino.h>


#define PIN_EEF_SERVO 21
#define PIN_EEF_A  18
#define PIN_EEF_B  19
#define PIN_HOME_ALHPA 16
#define PIN_HOME_BETA 17

// #define PIN_MOVER_LEFT_SERVO 4
// #define PIN_MOVER_RIGHT_SERVO 5


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


/*==========================================================================
 * The sketch shows how to move more than one motor. 
 * 
 * If more than one motor is moved by one controller all motors will arrive at 
 * their targets at the same time. E.g., if the motors are part of a 
 * x/y transport system, the transport move on a straight diagonal line to the
 * target coordinates.
 * 
 * The sketch also shows examples how the motor properties are set up
 *
 * A 1/16 microstep driver is assumed. You probably want to adjust speed, 
 * acceleration and distances if you are using a driver with another microstep 
 * resolution.
 ===========================================================================*/

// #include "ESP32Step/TeensyStep.h"

// Stepper motor_1(2, 3);   //STEP pin =  2, DIR pin = 3
// Stepper motor_2(9,10);   //STEP pin =  9, DIR pin = 10
// Stepper motor_3(14,15);  //STEP pin = 14, DIR pin = 15

StepControl controller;

void setup2()
{
  // setup the motors 
  //  motor_1
  //   .setMaxSpeed(50000)       // steps/s
  //   .setAcceleration(200000); // steps/s^2 
  
  // motor_2
  //   .setMaxSpeed(50000)       // steps/s
  //   .setAcceleration(200000); // steps/s^2 
  // motor_3
  //   //.setPullInSpeed(300)      // steps/s     currently deactivated...
  //   .setMaxSpeed(10000)       // steps/s
  //   .setAcceleration(50000)   // steps/s^2     
  //   .setStepPinPolarity(LOW); // driver expects active low pulses
}

void loop2() 
{  
  // constexpr int spr = 16*200;  // 3200 steps per revolution
  
  // // lets shake    
  // for(int i = 0; i < 5; i++)
  // {
  //   motor_1.setTargetRel(spr/4); // 1/4 revolution
  //   controller.move(motor_1);  

  //   motor_1.setTargetRel(-spr/4);
  //   controller.move(motor_1);  
  // }
  // delay(500);
  
  // // move motor_1 to absolute position (10 revolutions from zero)
  // // move motor_2 half a revolution forward  
  // // both motors will arrive their target positions at the same time
  // motor_1.setTargetAbs(10*spr);
  // motor_2.setTargetRel(spr/2);
  // controller.move(motor_1, motor_2);

  // // now move motor_2 together with motor_3
  // motor_2.setTargetRel(300);
  // motor_3.setTargetRel(-800);
  // controller.move(motor_2, motor_3);

  // // move all motors back to their start positions
  // motor_1.setTargetAbs(0);
  // motor_2.setTargetAbs(0);
  // motor_3.setTargetAbs(0);
  // controller.move(motor_1, motor_2, motor_3);
 
  // delay(1000);
}



// RobotAction* arm_action;
Arm::Arm(){
  // __Mcp23018 = &Mcp23018::getInstance();
  Servo sv = Servo();
  sv.attach(PIN_EEF_SERVO);
  eefServo = &sv ;
  
  // sv=Servo();
  // sv.attach(PIN_MOVER_LEFT_SERVO);
  // mover_left_Servo = &sv;

  // sv= Servo();
  // sv.attach(PIN_MOVER_RIGHT_SERVO);
  // mover_right_Servo = &sv;

  pinMode(PIN_EEF_A, OUTPUT);
  digitalWrite(PIN_EEF_A, HIGH);
  pinMode(PIN_EEF_B,OUTPUT);
  digitalWrite(PIN_EEF_B, HIGH);
  pinMode(PIN_BETA_ENABLE, OUTPUT);
  digitalWrite(PIN_BETA_ENABLE, HIGH);

  pinMode(PIN_HOME_ALHPA,INPUT_PULLUP);
  pinMode(PIN_HOME_BETA, INPUT_PULLUP);

  // AccelStepper stepper = AccelStepper(AccelStepper::MotorInterfaceType::DRIVER, 
  //                                     PIN_ALPHA_STEP, PIN_ALPHA_DIR);
  // stepper_alpha = & stepper;
  // steppers.addStepper(stepper);

  // stepper = AccelStepper(AccelStepper::MotorInterfaceType::DRIVER, 
  //                       PIN_BETA_STEP,PIN_BETA_DIR);
  // stepper_beta = & stepper;
  // steppers.addStepper(stepper);

  Stepper stepper = Stepper(PIN_ALPHA_STEP, PIN_ALPHA_DIR);
  stepper_alpha = &stepper;
  stepper_alpha->setTargetRel(3000);
  steppers.move(stepper_alpha);
  // steppers.attachStepper(stepper);

  // stepper = Stepper(PIN_BETA_STEP, PIN_BETA_DIR);
  // stepper_beta = & stepper;
  // steppers.addStepper(stepper);

  // stepper_alpha->setMaxSpeed(MOTOR_MAX_SPEED);
  // stepper_beta->setMaxSpeed(MOTOR_MAX_SPEED);

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
  homing_axis = axis;
  homed = false;
  if (axis == 4 ){
    home_pin = PIN_HOME_ALHPA;
    // stepper = stepper_alpha;
    Serial.println("Home Alpha");
  }
  else {
    // axis == 5
    home_pin = PIN_HOME_BETA;
    // stepper = stepper_beta;
    Serial.println("Home Beta");
  }
  HomeSpin();
}

void Arm::HomeSpin(){
  if (homed){
    __arm_action->bytes[0] = 0;
    return;
  }
  // stepper->setCurrentPosition(0);
  // stepper->move(100);
  homed = digitalRead(home_pin);
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
  // steppers.moveTo(angles);
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
  if ((action_code & (1<<2)) >= 1){
    MoveTo(pAction->Arm.pickup_x, pAction->Arm.pickup_y);
    SetEffector(Lower);
    SetEffector(Suck);
    SetEffector(Higher);
  }
  if ((action_code & (1<<3)) >= 1){
    MoveTo(pAction->Arm.place_x, pAction->Arm.place_y);
    SetEffector(Lower);
    SetEffector(Release);
    SetEffector(Higher);
    SetEffector(Sleep);
  }
  if ((action_code & (1<<4)) >= 1){
    MoveTo(pAction->Arm.park_x, pAction->Arm.park_y);
    SetEffector(Sleep);
  }
}

void Arm::Setup(RobotAction* pAction){
  __arm_action = pAction;
  homed = true;  //????????????
}

void Arm::SpinOnce(){
  // steppers.run();
  return;
  if (!homed)
    HomeSpin();
  // else if (!steppers.run())
  //   __arm_action->bytes[0] = 1;
}
