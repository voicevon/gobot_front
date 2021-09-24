#include "gobot_house.h"

#define PIN_ALPHA_ENABLE 18
#define PIN_BETA_ENABLE 16
#define PIN_MICRIO_STEP_2 21
#define PIN_MICRIO_STEP_1 22
#define PIN_MICRIO_STEP_0 23



#define MOTOR_MAX_SPEED 289
#define HOME_POSITION_ALPHA 0
#define HOME_POSITION_BETA 1

#define LINK_A 75
#define LINK_B 75

#define STEPS_PER_RAD 326;   //2048 / 2*Pi

// https://lastminuteengineers.com/28byj48-stepper-motor-arduino-tutorial/


GobotHouse::GobotHouse(){
}

void GobotHouse::Setup(RobotAction* pAction, int segments){

    __house_action = pAction;
    __segments = segments;
    __map.setup();
}



void GobotHouse::HomeAllAxises(){
  while(!objHomeHelper_alpha.IsTriged()){
    objStepper_beta.setTargetRel(100);
    objStepControl.move(objStepper_beta);
  }
  objAxis_Beta.SetCurrentPosition(HOME_POSITION_BETA);

  while(!objHomeHelper_alpha.IsTriged()){
    objStepper_alpha.setTargetRel(100);
    objStepControl.move(objStepper_alpha);
  }
  objAxis_Alpha.SetCurrentPosition(HOME_POSITION_ALPHA);
  
  this->commuDevice->OutputMessage("Home is done.....");
}



// Head is a position name, The 5 bar arm will pick up stone from there.
void GobotHouse::MoveStone_FromRoomToHead(uint8_t room_id){
  __Move_fromNeck_toDoor(room_id,true);
  __Move_fromRoom_toDoor(room_id,false);
  __Enable_eefCoil(true);
  __Move_fromRoom_toDoor(room_id, true);
  __Move_fromNeck_toDoor(0, false);
  __Move_fromHead_toNeck(false);
  __Enable_eefCoil(false);
  __Move_fromHead_toNeck(true);
}

void GobotHouse::MoveStone_FromHeadToRoom(uint8_t room_id){
  __Move_fromNeck_toDoor(0, false);  // 0 is useless.
  __Move_fromHead_toNeck(false);
  __Enable_eefCoil(true);
  __Move_fromHead_toNeck(true);
  __Move_fromNeck_toDoor(room_id, true);
  __Move_fromRoom_toDoor(room_id, false);
  __Enable_eefCoil(false);
  __Move_fromRoom_toDoor(room_id, true);
}

void GobotHouse::__Enable_eefCoil(bool enable){

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
// ik_position GobotHouse::ik(float x, float y){
//   ik_position ret;   //is risk here?
//   float rr1= x*x +y*y;
  
//   float beta = acosf((LINK_A * LINK_A + LINK_B * LINK_B -  rr1 ) / (2* LINK_A * LINK_B));
//   float r1 = sqrtf(rr1);
//   float alpha_eef = acosf(x/r1);
//   float alpha_link = acosf((LINK_A * LINK_A + rr1 - LINK_B * LINK_B)/( 2*LINK_A * r1));
//   float alpha = alpha_eef + alpha_link;
//   ret.alpha = alpha * STEPS_PER_RAD;
//   ret.beta =  beta * STEPS_PER_RAD; 
//   return ret;
// }
IkPositionBase* GobotHouse::IK(FkPositionBase* fk){

}
FkPositionBase* GobotHouse::FK(IkPositionBase* ik){

}


void GobotHouse::__Move_fromHead_toNeck(bool forwarding){
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
    // MoveTo(x, 0);
    this->ActuatorMoveTo_FK(x,0);
  }
}

void GobotHouse::__Move_fromRoom_toDoor(uint8_t room_id, bool forwarding){
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
    // MoveTo(x,y);
    this->ActuatorMoveTo_FK(x,y);
  }
}

// This is almost a  rotation, because beta should be no changing.
void GobotHouse::__Move_fromNeck_toDoor(uint8_t room_id, bool forwarding){
  float x = __map.doors[room_id].x;
  float y = __map.doors[room_id].y;
  if (!forwarding){
    x = __map.neck.x;
    y = __map.neck.y;    
  }
  this->ActuatorMoveTo_FK(x,y);
}

void GobotHouse::init_gpio(){
    pinMode(PIN_ALPHA_ENABLE, OUTPUT);
    pinMode(PIN_BETA_ENABLE, OUTPUT);
    pinMode(PIN_MICRIO_STEP_0, OUTPUT);
    pinMode(PIN_MICRIO_STEP_1, OUTPUT);
    pinMode(PIN_MICRIO_STEP_2, OUTPUT);

    digitalWrite(PIN_ALPHA_ENABLE, LOW);
    digitalWrite(PIN_BETA_ENABLE, LOW);
    digitalWrite(PIN_MICRIO_STEP_0, LOW);
    digitalWrite(PIN_MICRIO_STEP_1, LOW);
    digitalWrite(PIN_MICRIO_STEP_2, LOW);

}
void GobotHouse::Init_Linkage(){
  init_gpio();
  this->commuDevice = &this->objCommuUart; 
  this->objHomeHelper_alpha.LinkAxis(&this->objAxis_Alpha);
  this->objHomeHelper_beta.LinkAxis(&this->objAxis_Beta);

  this->objAxis_Alpha.LinkAcuator(&this->objActuator_Alpha);
  this->objActuator_Alpha.LinkDriver(nullptr);
  this->objActuator_Alpha.LinkSensorHelper(nullptr);

  this->objAxis_Beta.LinkAcuator(&this->objActuator_Beta);
  this->objActuator_Beta.LinkDriver(nullptr);
  this->objActuator_Beta.LinkSensorHelper(nullptr);

}

void GobotHouse::RunG1(Gcode* gcode) {

}


void GobotHouse::ActuatorMoveTo_FK(float x, float y){

}
void GobotHouse::ActuatorMoveTo_IK(int32_t a, int32_t b){
  
}
