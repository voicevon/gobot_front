#include "house.h"

// #define PIN_HOME_ALHPA 35     //??
// #define PIN_ALPHA_DIR 19
// #define PIN_ALPHA_STEP 5
// #define PIN_ALPHA_ENABLE 18

// #define PIN_HOME_BETA 34      //??
// #define PIN_BETA_DIR 17
// #define PIN_BETA_STEP 4
// #define PIN_BETA_ENABLE 16

// #define ENDER_COIL 32
// #define ENDER_COIL_EXT 33





// #define EEF_HALL_SENSOR_FRONT 22
// #define EEF_HALL_SENSOR_BACK 22
// #define EEF_HALL_SENSOR_LEFT 22
// #define EEF_HALL_SENSOR_RIGHT 22

#define MOTOR_MAX_SPEED 289
#define HOMED_POSITION_ALPHA 0
#define HOMED_POSITION_BETA 1

#define LINK_A 75
#define LINK_B 75

#define STEPS_PER_RAD 326;   //2048 / 2*Pi

// https://lastminuteengineers.com/28byj48-stepper-motor-arduino-tutorial/


House::House(){
  
}

void House::Setup(RobotAction* pAction, int segments){

    __house_action = pAction;
    __segments = segments;
    __map.setup();

    // stepper_alpha = new Stepper(PIN_ALPHA_STEP, PIN_ALPHA_DIR);
    // stepper_beta = new Stepper(PIN_BETA_STEP, PIN_BETA_DIR);

    // stepper_alpha->setMaxSpeed(MOTOR_MAX_SPEED);
    // stepper_beta->setMaxSpeed(MOTOR_MAX_SPEED);
    // steppers = new StepControl();        
}



void House::HomeAllAxises(){
  // uint8_t homing_axis = axis;
  // uint8_t home_pin = PIN_HOME_BETA;
  // Stepper* homing_stepper = stepper_beta;

  // if (axis == HOUSE_ALPHA_AXIS ){
  //   home_pin = PIN_HOME_ALHPA;
  //   homing_stepper = stepper_alpha;
  //   Serial.print("\nStart Homing Alpha");
  // }
  // else {
  //   // axis == 1
  //   // homing_stepper = stepper_beta;
  //   Serial.print("\nStart Homing Beta");
  // }
  // homing_stepper->setTargetRel(-500);  //Any nagtive number is ok!  ??
  // steppers->move(*homing_stepper);
  // while (steppers->isRunning()){
  //   //Seems alway return false of isRunning()
  //   Serial.print("r");
  // };

  // __HomeSpin(homing_stepper, home_pin);
  // if (homing_axis == HOUSE_ALPHA_AXIS){
  //   stepper_alpha->setPosition(HOMED_POSITION_ALPHA);
  // }
  // else{
  //   stepper_beta->setPosition(HOMED_POSITION_BETA);
  // }
  // __house_action->bytes[0] = 0;
  // Serial.print("\n Home one axis is Done.......");
}

void House::__HomeSpin(Stepper* homing_stepper, uint8_t home_pin ){
  // uint8_t flags = 0x0;
  // // Serial.print("\nHome spin got started.............\n");
  // while (flags != 0xff){    // a filter for 16 times of hall sensor is trigged.
  //   homing_stepper->setTargetRel(10000);
  //   steppers->move(*homing_stepper);  
  //   flags <<= 1;
  //   // flags |= !digitalRead(home_pin);   // low level is trigged !
  //   Serial.print(".");
  // }
}

void House::SpinOnce(){
}



// Head is a position name, The 5 bar arm will pick up stone from there.
void House::MoveStone_FromRoomToHead(uint8_t room_id){
  __Move_fromNeck_toDoor(room_id,true);
  __Move_fromRoom_toDoor(room_id,false);
  __Enable_eefCoil(true);
  __Move_fromRoom_toDoor(room_id, true);
  __Move_fromNeck_toDoor(0, false);
  __Move_fromHead_toNeck(false);
  __Enable_eefCoil(false);
  __Move_fromHead_toNeck(true);
}

void House::MoveStone_FromHeadToRoom(uint8_t room_id){
  __Move_fromNeck_toDoor(0, false);  // 0 is useless.
  __Move_fromHead_toNeck(false);
  __Enable_eefCoil(true);
  __Move_fromHead_toNeck(true);
  __Move_fromNeck_toDoor(room_id, true);
  __Move_fromRoom_toDoor(room_id, false);
  __Enable_eefCoil(false);
  __Move_fromRoom_toDoor(room_id, true);
}

void House::__Enable_eefCoil(bool enable){

}


/*

    House                Y+
                         ^
             r0          |
          r1     d0      |
        r2      d1       |
      r3       d3  d2    |               
      -----------------(0,0)------Neck----------Head    --> X+
      r4      d4  d5     |
       r5       d6       |
         r6     d7       |
          r7             |

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


void House::__Move_fromHead_toNeck(bool forwarding){
  float x1 = __map.head.x;
  float x2 = __map.neck.x;
  if(!forwarding){
    //from neck to head
    x1 = __map.neck.x;
    x2 = __map.head.x;
  }
  float distance = x2 - x1;
  float dx = distance / __segments ;
  for(int segment= 0; segment < __segments; segment++){
    float x = x1 + dx * segment;
    MoveTo(x, 0);
  }
}

void House::__Move_fromRoom_toDoor(uint8_t room_id, bool forwarding){
    //from room to door, now is at room.
  float x1 = __map.rooms[room_id].x;
  float y1 = __map.rooms[room_id].y;
  float x2 = __map.doors[room_id].x;
  float y2 = __map.doors[room_id].y;

  if (!forwarding){
    //from door to room, now is at door.
    x1 = __map.doors[room_id].x;
    y1 = __map.doors[room_id].y;
    x2 = __map.rooms[room_id].x;
    y2 = __map.rooms[room_id].y;
  }

  // float distance = __map.distance_room_to_door[room_id];
  float dx = (x2 - x1) / __segments;
  float dy = (y2 - y1) / __segments;
  for(int segment= 0; segment < __segments; segment++){
    float x = x1 + dx * segment;
    float y = y1 + dy * segment;
    MoveTo(x,y);
  }
}

// This is almost a  rotation, because beta should be no changing.
void House::__Move_fromNeck_toDoor(uint8_t room_id, bool forwarding){
  float x = __map.doors[room_id].x;
  float y = __map.doors[room_id].y;
  if (!forwarding){
    x = __map.neck.x;
    y = __map.neck.y;    
  }

  MoveTo(x,y);
}
