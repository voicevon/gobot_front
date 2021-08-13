#include "house.h"

// Total occupies 5+5 + 2 = 12 Pins
#define PIN_HOME_ALHPA 12
#define PIN_ALPHA_DIR 12
#define PIN_ALPHA_STEP 12
#define PIN_ALPHA_ENABLE 12

#define PIN_HOME_BETA 12
#define PIN_BETA_DIR 12
#define PIN_BETA_STEP 12
#define PIN_BETA_ENABLE 12

#define ENDER_COIL 24
#define EEF_HALL_SENSOR_FRONT 22
#define EEF_HALL_SENSOR_BACK 22
#define EEF_HALL_SENSOR_LEFT 22
#define EEF_HALL_SENSOR_RIGHT 22

#define MOTOR_MAX_SPEED 2289
#define HOMED_POSITION_ALPHA 0
#define HOMED_POSITION_BETA 1

#define ALPHA_AXIS 0
#define BETA_AXIS 1

// https://lastminuteengineers.com/28byj48-stepper-motor-arduino-tutorial/


House::House(){
  // Path from head to neck
  uint16_t path8[10] = {11,22,  33,44,  55,66, 77,88, 99, 10};
  for(uint16_t i=0; i< sizeof(path8) /2; i++){
    Path[8][i] = path8[i];
  }

  // Path to house of id=0
  uint16_t path0[10] = {11,22,  33,44,  55,66, 77,88, 99, 10};
  for(uint16_t i=0; i< sizeof(path0) /2; i++){
    Path[8][i] = path0[i];
  }
  // Path to house of id=1
  // Path to house of id=2
  // Path to house of id=3
  // Path to house of id=4
  // Path to house of id=5
  // Path to house of id=6
  // Path to house of id=7
  // Path to house of id=8

}
void House::Setup(RobotAction* pAction){
    __house_action = pAction;
    
    pinMode(PIN_BETA_ENABLE, OUTPUT);
    digitalWrite(PIN_BETA_ENABLE, HIGH);

    pinMode(PIN_HOME_ALHPA,INPUT_PULLUP);
    pinMode(PIN_HOME_BETA, INPUT_PULLUP);

    stepper_alpha = new Stepper(PIN_ALPHA_STEP, PIN_ALPHA_DIR);
    stepper_beta = new Stepper(PIN_BETA_STEP, PIN_BETA_DIR);

    stepper_alpha->setMaxSpeed(MOTOR_MAX_SPEED);
    stepper_beta->setMaxSpeed(MOTOR_MAX_SPEED);
    digitalWrite(PIN_BETA_ENABLE, LOW);
    steppers = new StepControl();
}

void House::Home(uint8_t axis){
  uint8_t homing_axis = axis;
  uint8_t home_pin = PIN_HOME_BETA;
  Stepper* homing_stepper = stepper_beta;

  if (axis == ALPHA_AXIS ){
    home_pin = PIN_HOME_ALHPA;
    homing_stepper = stepper_alpha;
    // stepper = stepper_alpha;
    Serial.print("\nStart Homing Alpha");
  }
  else {
    // axis == 1
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
  if (homing_axis == ALPHA_AXIS){
    stepper_alpha->setPosition(HOMED_POSITION_ALPHA);
  }
  else{
    stepper_beta->setPosition(HOMED_POSITION_BETA);
  }
  __house_action->bytes[0] = 0;
  Serial.print("\n Home one axis is Done.......");
}

void House::__HomeSpin(Stepper* homing_stepper, uint8_t home_pin ){
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
void House::SpinOnce()
{
}

motor_position House::ik(int x, int y){
  Serial.println("Error, No IK is avaliable of House object.");
  motor_position ret;   //is risk here?
  return ret;
}


// Head is a position name, The 5 bar arm will pick up stone from there.

void House::MoveStone_FromHouseToHead(uint8_t house_id){
  // Move to bottom of house
  __MoveOut_FromHouse(house_id);
  Home(BETA_AXIS);
  // Alpha to Neck.
  // stepper_alpha->setPosition(123);
  // Alpha , Beta to Head.

}
void House::MoveStone_FromHeadToHouse(uint8_t house_id){
  
}

void House::__MoveOut_FromHouse(uint8_t house_id){
  for(int i=0; i<5; i++){
    a = Path[house_id][i];
    b = Path[house_id][i+1];
    stepper_alpha->setTargetAbs(a);
    stepper_beta->setTargetAbs(b);
    steppers->move();
  }
}

void House::__MoveIn_ToHouse(uint8_t house_id){
  // Move to bottom of house

  // Draw stone out , follow linear way.

  
}

void House::__Move_FromNeck_ToHead(){
  
}
void House::__Move_FromHead_ToNeck(){
  
}
