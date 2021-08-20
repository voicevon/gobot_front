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

#define STEPS_PER_RAD 2345;

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

    __map.setup(20);
    // __map.neck.y = 0;
    // __map.neck.x = 60.0;
    // __map.head.y = 0;
    // __map.head.x = 148.93;

    // __map.rooms[0].x = -119.51;
    // __map.rooms[0].y = 126.38;
    // __map.rooms[1].x = -111.9;
    // __map.rooms[1].y = 95.34;
    // __map.rooms[2].x = -98.18;
    // __map.rooms[2].y = 61.4;
    // __map.rooms[3].x = -78.79;
    // __map.rooms[3].y = 25.59;

    // __map.rooms[4].x = __map.rooms[3].x;
    // __map.rooms[4].y = - __map.rooms[3].y;
    // __map.rooms[5].x = __map.rooms[2].x;
    // __map.rooms[5].y = - __map.rooms[2].y;
    // __map.rooms[6].x = __map.rooms[1].x;
    // __map.rooms[6].y = - __map.rooms[1].y;
    // __map.rooms[7].x = __map.rooms[0].x;
    // __map.rooms[7].y = - __map.rooms[0].y;

    // __map.doors[0].x = -49.46;
    // __map.doors[0].y = 59.07;
    // __map.doors[1].x = -81.22;
    // __map.doors[1].y = 56.12;
    // __map.doors[2].x = -104.11;
    // __map.doors[2].y = 40.67;
    // __map.doors[3].x = -117.25;
    // __map.doors[3].y = 17.95;
    // __map.doors[4].x = __map.doors[3].x;
    // __map.doors[4].y = - __map.doors[3].y;
    // __map.doors[5].x = __map.doors[2].x;
    // __map.doors[5].y = - __map.doors[2].y;
    // __map.doors[6].x = __map.doors[1].x;
    // __map.doors[6].y = - __map.doors[1].y;
    // __map.doors[7].x = __map.doors[0].x;
    // __map.doors[7].y = - __map.doors[0].y;

        
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
motor_position House::ik(float x, float y){
  motor_position ret;   //is risk here?
  float rr1= x*x +y*y;
  
  float beta = acosf((LINK_A * LINK_A + LINK_B * LINK_B -  rr1 ) / (2* LINK_A * LINK_B));
  float r1 = sqrtf(rr1);
  float alpha_eef = acosf(x/r1);
  float alpha_link = acosf((LINK_A * LINK_A + rr1 - LINK_B * LINK_B)/( 2*LINK_A * r1));
  float alpha = alpha_eef + alpha_link;
  ret.alpha = alpha * STEPS_PER_RAD;
  ret.beta =  beta * STEPS_PER_RAD; 
  return ret;
}


void House::__Move_fromHead_toNeck(bool reverse){
  float x1 = __map.head.x;
  float x2 = __map.neck.x;
  if(reverse){
    //to head
    float x2 = __map.head.x;
    float x1 = __map.neck.x;
  }
  float distance = x2 - x1;
  float dx = distance / __map.get_segments() ;
  for(int segment= 0; segment < __map.get_segments(); segment++){
    float x = x1 + dx * segment;
    MoveTo(x, 0);
    }
  }
}

void House::__Move_fromDoor_toRoom(uint8_t house_id, bool reverse){
    //from room to door, now is at room.
  float x1 = __map.rooms[house_id].x;
  float y1 = __map.rooms[house_id].y;
  float x2 = __map.doors[house_id].x;
  float y2 = __map.doors[house_id].y;

  if (reverse){
    //from house to door, now is at house.
    x2 = __map.rooms[house_id].x;
    y2 = __map.rooms[house_id].y;
    x1 = __map.doors[house_id].x;
    y1 = __map.doors[house_id].y;
  }

  float distance = __map.distance_room_to_door[house_id];
  float dx = __map.get_dx_distance(house_id) ;
  float dy = __map.get_dy_distance(house_id) ;
  for(int segment= 0; segment < __map.get_segments(); segment++){
    float x = x1 + dx * segment;
    float y = y1 + dy * segment;
    MoveTo(x,y);
    }
  }
}

// This is almost a  rotation, because beta should be no changing.
void House::__Move_fromNeck_toDoor(uint8_t house_id, bool reverse){
  float x = __map.doors[house_id].x;
  float y = __map.doors[house_id].y;
  if (reverse){
    x = __map.neck.x;
    y = __map.neck.y;    
  }

  MoveTo(x,y);
}
