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

// https://lastminuteengineers.com/28byj48-stepper-motor-arduino-tutorial/


House::House(){
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

}

void House::DrawStone(uint8_t house_id)
{
}

void House::MoveStoneToTarget(uint8_t start_coil)
{

}
