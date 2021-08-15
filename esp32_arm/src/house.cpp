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

#define LINK_A 75
#define LINK_B 75

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



// Head is a position name, The 5 bar arm will pick up stone from there.

void House::MoveStone_FromHouseToHead(uint8_t house_id){
  __Move_fromNeck_toDoor(house_id,true);
  __Move_fromDoor_toHouse(house_id,true);
  __Enable_eefCoil(true);
  __Move_fromDoor_toHouse(house_id, true);
  __Move_fromNeck_toDoor(0,true);
  __Move_fromHead_toNeck(true);
  __Enable_eefCoil(false);
  __Move_fromHead_toNeck(false);
}

void House::MoveStone_FromHeadToHouse(uint8_t house_id){
  __Move_fromDoor_toHouse(0,true);  // 0 is useless.
  __Move_fromHead_toNeck(true);
  __Enable_eefCoil(true);
  __Move_fromHead_toNeck(house_id);
  __Move_fromNeck_toDoor(house_id, false);
  __Move_fromDoor_toHouse(house_id, false);
  __Enable_eefCoil(false);
  __Move_fromDoor_toHouse(house_id, true);
}

void House::__Enable_eefCoil(bool enable){

}


/*

    House

             0
          1
        2
      3                        
                 (O,0)                   Neck----------Head
      4
       5
         6
          7

*/
motor_position House::ik(int x, int y){
  motor_position ret;   //is risk here?
  if(x>0){
    
  }
  return ret;
}


void House::__Move_fromHead_toNeck(bool reverse){
  float pos[4]={80,0,  149,0};   //Neck(x,y),  Head(x,y)
  float x = pos[0];
  float y = pos[1];
  if(reverse){
    //to head
    x = pos[2];
    y = pos [3];
  }
  MoveTo(x,y);
}

void House::__Move_fromDoor_toHouse(uint8_t house_id, bool reverse){
  float x = 100;
  float y = 100;
  int steps_count = 20;
  int x_step_distance = 50 / steps_count;
  if (reverse){
    for(int dY=0; dY<steps_count; dY++){
      x = x_step_distance * steps_count + x_step_distance * dY;
      MoveTo(x,y);
    }
  }else{
    for(int dY= steps_count-1 ; dY>=0; dY--){
      x = x_step_distance * steps_count + x_step_distance * dY;
      MoveTo(x,y);
      }
  }
}

// This is a rotation.
void House::__Move_fromNeck_toDoor(uint8_t house_id, bool reverse){
  float ANGLE[8] = {0,1,2,3,4,5,6,7};
  float pos[16] = {0,0,1,1,2,2,3,3,4,4,5,5,6,6,7,7};

  // home beta firstly
  Home(BETA_AXIS);
  // do rotation 
  int angle = ANGLE[house_id];
  if (reverse){
    stepper_alpha->setTargetAbs(100);
  }else{
    stepper_alpha->setTargetAbs(angle);
  }
  // move to door
  float x = pos[house_id];
  float y = pos[house_id + 1];
  MoveTo(x,y);
}
